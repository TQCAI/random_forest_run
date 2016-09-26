#ifndef RFR_BINARY_NODES_HPP
#define RFR_BINARY_NODES_HPP

#include <vector>
#include <deque>
#include <array>
#include <tuple>
#include <sstream>
#include <algorithm>

#include "rfr/data_containers/data_container_base.hpp"
#include "rfr/data_containers/data_container_utils.hpp"
#include "rfr/nodes/temporary_node.hpp"
#include "rfr/util.hpp"

#include "cereal/cereal.hpp"
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>


#include <iostream>


namespace rfr{ namespace nodes{
	
/** \brief The node class for regular k-ary trees.
 * 
 * In a regular k-ary tree, every node has either zero (a leaf) or exactly k-children (an internal node).
 * In this case, one can try to gain some speed by replacing variable length std::vectors by std::arrays.
 * 
 */
template <int k, typename split_type, typename rng_t, typename num_t = float, typename response_t = float, typename index_t = unsigned int>
class k_ary_node{
  private:
	index_t parent_index;
	bool is_leaf;

	// for leaf nodes
	std::vector<response_t> response_values;

	// for internal_nodes
	std::array<index_t, k> children;
	std::array<num_t, k> split_fractions;
	split_type split;
	
  public:

  	/* serialize function for saving forests */
  	template<class Archive>
	void serialize(Archive & archive) {
		archive( parent_index, is_leaf, response_values, children, split_fractions, split); 
	}

  
	/** \brief If the temporary node should be split further, this member turns this node into an internal node.
	*
	* 
	* \param tmp_node a temporary_node struct containing all the important information. It is not changed in this function.
	* \param data a refernce to the data object that is used
	* \param features_to_try vector of allowed features to be used for this split
	* \param nodes reference to vector containing all processed nodes
	* \param tmp_nodes reference to vector containing all temporary nodes that still have to be checked
	*
	* \return num_t the loss of the split
	*/ 
	num_t make_internal_node(rfr::nodes::temporary_node<num_t, index_t> &tmp_node,
							const rfr::data_containers::data_container_base<num_t, response_t, index_t> &data,
							std::vector<index_t> &features_to_try,
							index_t num_nodes,
							std::deque<rfr::nodes::temporary_node<num_t, index_t> > &tmp_nodes,
							rng_t &rng){
		is_leaf = false;
		response_values.clear();
		parent_index = tmp_node.parent_index;
		std::array<typename std::vector<index_t>::iterator, k+1> split_indices_it;
		num_t best_loss = split.find_best_split(data, features_to_try, tmp_node.data_indices, split_indices_it,rng);
	
		//check if a split was found
		// note: if the number of features to try is too small, there is a chance that the data cannot be split any further
		if (best_loss <  std::numeric_limits<num_t>::infinity()){
			// create an empty node, and a tmp node for each child
			for (index_t i = 0; i < k; i++){
				tmp_nodes.emplace_back(num_nodes+i, tmp_node.node_index, tmp_node.node_level+1, split_indices_it[i], split_indices_it[i+1]);
				split_fractions[i] = ((num_t) std::distance(split_indices_it[i],split_indices_it[i+1])) / 
									 ((num_t) std::distance(split_indices_it[0],split_indices_it[k]));
				children[i] = num_nodes + i;
			}
		}
		else
			make_leaf_node(tmp_node, data);
		return(best_loss);
	}
	
	/** \brief  turns this node into a leaf node based on a temporary node.
	*
	*
	* \param tmp_node the internal representation for a temporary node.
	*
	*/
	void make_leaf_node(rfr::nodes::temporary_node<num_t, index_t> &tmp_node,
						const rfr::data_containers::data_container_base<num_t, response_t, index_t> &data){
		is_leaf = true;
		parent_index = tmp_node.parent_index;
		children.fill(0);
		
		response_values.resize(tmp_node.data_indices.size());
		for (size_t i = 0; i < tmp_node.data_indices.size(); i++){
			response_values[i] = data.response(tmp_node.data_indices[i]);
		}
		//to save some time
		std::sort(response_values.begin(), response_values.end());
	}	

	/* \brief function to check if a feature vector can be splitted */
	bool can_be_split(num_t *feature_vector){return(split.can_be_split(feature_vector));}

	/** \brief returns the index of the child into which the provided sample falls
	 * 
	 * \param sample a feature vector of the appropriate size (not checked!)
	 *
	 * \return index_t index of the child
	 */
	index_t falls_into_child(num_t * sample){
		if (is_leaf)
			return(0);
		return(children[split(sample)]);
	}

	void push_response_value ( num_t r){
		response_values.push_back(r);
	}
	
	void pop_repsonse_value (){
		response_values.pop_back();
	}
	

	/** \brief calculate the mean of all response values in this leaf
	 *
	 * \return num_t the mean, or NaN if the node is not a leaf
	 */
	num_t mean(){
		if (! is_leaf)
			return(std::numeric_limits<num_t>::quiet_NaN());
		
		num_t m = 0;
		for (auto v : response_values)
			m += v;

		return(m/((num_t) response_values.size()));
	}


	std::array<std::vector< std::vector<num_t> >, 2> compute_subspaces( std::vector< std::vector<num_t> > &subspace){
		return(split.compute_subspaces(subspace));
	}

	std::tuple<num_t, num_t, index_t> mean_variance_N (){
		if (! is_leaf)
			return(std::tuple<num_t, num_t, index_t>(	std::numeric_limits<num_t>::quiet_NaN(),
																std::numeric_limits<num_t>::quiet_NaN(),
																0));
		
		
		rfr::running_statistics<num_t> stats;

		for (auto v : response_values)
			stats(v);

		return( std::tuple<num_t, num_t, index_t> (stats.mean(), stats.variance(), stats.number_of_points()));
	}


	/** \brief to test whether this node is a leaf */
	bool is_a_leaf(){return(is_leaf);}
	/** \brief get the index of the node's parent */
	index_t parent() {return(parent_index);}
	
	index_t num_samples () {return(response_values.size());}
	
	
	/** \brief get indices of all children*/
	std::array<index_t, k> get_children() {return(children);}
	index_t get_child_index (index_t idx) {return(children[idx]);};

	std::array<num_t, k> get_split_fractions() {return(split_fractions);}
	num_t get_split_fraction (index_t idx) {return(split_fractions[idx]);};


	split_type get_split() {return(split);}


	/** \brief get reference to the response values*/	
	std::vector<response_t> const &responses (){ return( (std::vector<response_t> const &) response_values);}


	/** \brief prints out some basic information abouth the node*/
	void print_info(){
		if (is_leaf){
			std::cout<<"status: leaf\nresponse values: ";
			rfr::print_vector(response_values);
		}
		else{
			std::cout<<"status: internal node\n";
			std::cout<<"children: ";
			for (auto i=0; i < k; i++)
				std::cout<<children[i]<<" ";
			std::cout<<std::endl;
		}
	}


	/** \brief generates a label for the node to be used in the LaTeX visualization*/
	std::string latex_representation( int my_index){
		std::stringstream str;
			
		if (is_leaf){
			str << "{i = " << my_index << ": ";

			num_t s=0, ss=0;
			for (auto v: response_values){
				s += v;
				ss+= v*v;
			}

			auto N = response_values.size();
			str << "N = "<<N<<", mean = "<<s/N<<", variance = " << sqrt(ss/N - (s/N)*(s/N))<<"}";
			
		}
		else{
			str << "{ i = " << my_index << "\\nodepart{two} {";
			str << split.latex_representation() << "}},rectangle split,rectangle split parts=2,draw";
			
		}
		return(str.str());
	}
};


}} // namespace rfr::nodes
#endif
