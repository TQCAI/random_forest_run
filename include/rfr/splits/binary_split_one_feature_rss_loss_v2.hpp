#ifndef RFR_BINARY_SPLIT_RSS_HPP
#define RFR_BINARY_SPLIT_RSS_HPP

#include <vector>
#include <bitset>
#include <array>
#include <random>
#include <algorithm>
#include <string>
#include <sstream>
#include <iterator>


#include "cereal/cereal.hpp"
#include <cereal/types/bitset.hpp>
#include "rfr/data_containers/data_container_base.hpp"
#include "rfr/splits/split_base.hpp"
#include "rfr/data_containers/data_container_utils.hpp"
namespace rfr{ namespace splits{



template <	typename rng_t,
			typename index_t = unsigned int,
			typename num_t = float,
			typename response_t=float,
			unsigned int max_num_categories = 128>
class binary_split_one_feature_rss_loss: public rfr::splits::k_ary_split_base<2,rng_t, num_t, response_t, index_t> {
  private:
	
	index_t feature_index;	//!< split needs to know which feature it uses
	num_t num_split_value;	//!< value of a numerical split
	std::bitset<max_num_categories> cat_split_set;	//!< set of values for a categorical split
	
	typedef rfr::splits::data_info info_t;

  public:
  	
  	/* serialize function for saving forests */
  	template<class Archive>
	void serialize(Archive & archive){
		archive( feature_index, num_split_value, cat_split_set); 
	}
  	
  	
	/** \brief the implementation to find the best binary split using only one feature minimizing the RSS loss
	 *
	 * The best binary split is determined among all allowed features. For a continuous feature the split is a single value.
	 * For catergoricals, the split criterion is a "set" (actual implementation might use a different datatype for performance).
	 * In both cases the split is computed as efficiently as possible exploiting properties of the RSS loss (optimal split for categoricals
	 * can be found in polynomial rather than exponential time in the number of possible values).
	 * The constructor assumes that the data should be split. Testing whether the number of points and their values allow further splitting is checked by the tree
	 * 
	 * \param data the container holding the training data
	 * \param features_to_try a vector with the indices of all the features that can be considered for this split
	 * \param infos_begin iterator to the first (relevant) element in a vector containing the minimal information in tuples
	 * \param infos_end iterator beyond the last (relevant) element in a vector containing the minimal information in tuples
	 * \param info_split_its iterators into this vector saying where to split the data for the two children
	 * \param rng a random number generator instance
	 * \return num_t loss of the best found split
	 */
	 virtual num_t find_best_split(	const rfr::data_containers::data_container_base<num_t, response_t, index_t> &data,
									const std::vector<index_t> &features_to_try,
									std::vector<info_t>::iterator infos_begin,
									std::vector<info_t >::iterator infos_end,
									std::array<typename std::vector<info_t>::iterator, 3> &info_split_its,
									rng_t &rng){

				
		// precompute mean and variance of all responses
		rfr::util::weighted_running_statistics total_stat;
		for (auto it = infos_begin; it != infos_end; ++it){
			total_stat.push(it->response, it->weight);
		}
		
		num_t best_loss = std::numeric_limits<num_t>::infinity();

		for (index_t fi : features_to_try){ //! > uses C++11 range based loop

			num_t loss;
			num_t num_split_copy;
			std::bitset<max_num_categories> cat_split_copy;

			for (auto it = infos_begin; it != infos_end; ++it){
				it->feature = data.feature( fi, it->index);
			}

			index_t ft = data.get_type_of_feature(fi);
			// feature_type zero means that it is a continous variable
			if (ft == 0){
				// find best split for the current feature_index
				loss = best_split_continuous(infos_begin, infos_end, num_split_copy, total_stat ,rng);
			}
			// a positive feature type encodes the number of possible values
			if (ft > 0){
				// find best split for the current feature_index
				loss = best_split_categorical(infos_begin, infos_end, ft, cat_split_copy, total_stat, rng);
			}

			// check if this split is the best so far
			if (loss < best_loss){
				best_loss = loss;
				feature_index = fi;
				
				if (ft == 0){
					num_split_value = num_split_copy;
				}
				else{
					num_split_value = NAN;
					cat_split_set = cat_split_copy;
				}
					
			}
		}
		// now we have to rearrange the indices based on which leaf they fall into
		if (best_loss < std::numeric_limits<num_t>::infinity()){
			// the default values for the two split iterators
			info_split_its[0] = infos_begin;
			info_split_its[2] = infos_end;

			info_split_its[1] = std::partition (infos_begin, infos_end,
				[this, data] (info_t &arg){
					return !(this->operator() (data.feature(this->feature_index, arg)));
				});
		}
		return(best_loss);
	}


	/** \brief this operator tells into which child the given feature vector falls
	 * 
	 * \param feature_vector an array containing a valid (in terms of size and values!) feature vector
	 * 
	 * \return int whether the feature_vector falls into the left (false) or right (true) child
	 */
	virtual index_t operator() (num_t *feature_vector) { return(operator()(feature_vector[feature_index]));}
	
	/** \brief overloaded operator for just the respective feature value instead of the complete vector
	 * 
	 */
	virtual index_t operator() (num_t &feature_value) {
		// categorical feature
		if (std::isnan(num_split_value))
			return(! bool(cat_split_set[ int(feature_value)]));
		// standard numerical feature
		return(feature_value > num_split_value);
	}



	/** \brief member function to find the best possible split for a single (continuous) feature
	 * 
	 * 
	 * \param infos_begin iterator to the first (relevant) element in a vector containing the minimal information in tuples
	 * \param infos_end iterator beyond the last (relevant) element in a vector containing the minimal information in tuples
	 * \param split_value a reference to store the split (numerical) criterion
	 * \param total_stat a weighted_runnin_statistics object containing the statistics of all responses
	 * \param rng an pseudo random number generator instance
	 * 
	 * \return float the loss of this split
	 */
	virtual num_t best_split_continuous(
					std::vector<info_t>::iterator infos_begin,
					std::vector<info_t>::iterator infos_end,
					num_t &split_value,
					rfr::util::weighted_running_statistics right_stat,
					rng_t &rng){

		// first, sort the info vector by the feature
		std::sort(infos_begin, infos_end,
			[] (info_t &a, info_t &b) {return (a.feature < b.feature) ;});

		// first some temporary variables
		rfr::util::weighted_running_statistics left_stat;
		num_t best_loss = std::numeric_limits<num_t>::infinity();


		// now we can increase the splitting value to move data points from the right to the left child
		// this way we do not consider a split with everything in the right child
		auto it = infos_begin;
		while (it != infos_end){
			num_t psv = it->feature + 1e-6; // potential split value add small delta for numerical inaccuracy
			// combine data points that are very close
			do {
				left_stat.push(it->response, it->weight);
				right_stat.pop(it->response, it->weight);
				++it;
			} while ((it != infos_end) && (it->feature <= psv));
			
			// stop if all data points are now in the left child as this is not a meaningful split
			if (right_stat.number_of_points() == 0) {break;}

			// compute the loss
			num_t loss = 	left_stat.number_of_points() *  left_stat.variance() +
							right_stat.number_of_points() * right_stat.variance();

			// store the best split
			if (loss < best_loss){
				std::uniform_real_distribution<num_t> dist(0.0,1.0);
				best_loss = loss;
				split_value = psv + dist(rng)*(it->feature - psv);
			}
		}
		return(best_loss);
	}

	/** \brief member function to find the best possible split for a single (categorical) feature
	 * 
	 * \param infos_begin iterator to the first (relevant) element in a vector containing the minimal information in tuples
	 * \param infos_end iterator beyond the last (relevant) element in a vector containing the minimal information in tuples	 * 
	 * \param num_categories the feature type (number of different values)
	 * \param split_set a reference to store the split criterion
	 * \param S_y_right the sum of all the response values
	 * \param S_y2_right the sum of all squared response values
	 * \param rng an pseudo random number generator instance
	 * 
	 * \return float the loss of this split
	 */
	virtual num_t best_split_categorical(
									std::vector<info_t>::iterator infos_begin,
									std::vector<info_t>::iterator infos_end,
									index_t num_categories,
									std::bitset<max_num_categories> &split_set,
									rfr::util::weighted_running_statistics<num_t> right_stat,
									rng_t &rng){
		// auxiliary variables
		num_t best_loss = std::numeric_limits<num_t>::infinity();
		rfr::util::weighted_running_statistics<num_t> left_stat,

		std::vector<rfr::util::weighted_running_statistics<num_t> > cat_stats (num_categories);
		for (auto it = infos_begin; it != infos_end; ++it){
			// find the category for each entry as a proper int
			//! >assumes that the features for categoricals have been properly rounded so casting them to ints results in the right value!
			int cat = it->feature;
			// collect all the data to compute the loss
			cat_stats[cat].push(it->response);
		}

		// take care b/c certain categories might not be encountered (maybe there was a split on the same variable further up the tree...)
		// sort the categories by whether there were samples or not
		// std::partition rearranges the data using a boolean predicate into all that evaluate to true in front of all evaluating to false.
		// it even returns an iterator pointing to the first element where the predicate is false, how convenient :)
		auto empty_cat_stats_it = std::partition(cat_stats.begin(), cat_stats.end(),
						[](auto &stat){return(stat.number_of_points() > 0);});
		
		// sort the categories by their individual mean. only consider the ones with actual specimen here
		std::sort(	cat_stats.begin(), empty_cat_stats_it,
					[](auto &stat_a, auto &stat_b){return ( stat_a.mean() < stat_b.mean() );});		// C++11 lambda function, how exciting :)

		

// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================


		// it can happen that the node is not pure wrt the response, but the
		// feature at hand takes only one value in this node. By setting the
		// best_loss to the largest possible value, this split will not be chosen.
		
		if ( (N_right == 0) || (N_left == 0) )
			best_loss = 
		else
			best_loss = (S_y2_right - (S_y_right*S_y_right)/N_right)
							+ (S_y2_left - (S_y_left*S_y_left)/N_left);

		current_loss = best_loss;

		// now move one category at a time to the left child and recompute the loss
		for (auto it1 = it_best_split; it1 != empty_categories_it; it1++){
			S_y_left  += S_y[*it1];
			S_y_right -= S_y[*it1];

			S_y2_left  += S_y2[*it1];
			S_y2_right -= S_y2[*it1];

			N_left  += N_points_in_category[*it1];
			N_right -= N_points_in_category[*it1];


			// catch divide by zero as they are invalid splits anyway
			// becomes important if only one or two categories have specimen here!
			if ( (N_right != 0) && (N_left != 0) ){
				current_loss 	= (S_y2_right - (S_y_right*S_y_right)/N_right)
								+ (S_y2_left - (S_y_left*S_y_left)/N_left);

				// keep the best split
				if (current_loss < best_loss){
					best_loss = current_loss;
					it_best_split = it1;
					it_best_split++;
				}
			}
		}

		// store the split set for the left leaf
		split_set.reset();
		for (auto it1 = category_ranking.begin(); it1 != it_best_split; it1++)
			split_set.set(*it1);

		// add unobserved values randomly to the split_set
		if (empty_categories_it != category_ranking.end()){
			std::bernoulli_distribution dist;

			for (auto it1 = empty_categories_it; it1 != category_ranking.end(); it1++){
				if (dist(rng))
					split_set.set(*it1);
			}
		}
		return(best_loss);
	}


	virtual void print_info(){
		if(std::isnan(num_split_value)){
			std::cout<<"split: f_"<<feature_index<<" in {";
			for (size_t i = 0; i < max_num_categories; i++)
				if (cat_split_set[i]) std::cout<<i<<", ";
			std::cout<<"\b\b}\n";			
		}
		else
			std::cout<<"split: f_"<<feature_index<<" <= "<<num_split_value<<"\n";
	}

	/** \brief member function to create a string representing the split criterion
	 * 
	 * \return std::string a label that characterizes the split
	 */	
	virtual std::string latex_representation(){
		std::stringstream str;

		if (std::isnan(num_split_value)){
			auto i = 0u;
			while (cat_split_set[i] == 0)
				i++;
			str << "$f_{" << feature_index << "} \\in \\{"<<i;
			
			for (i++; i < max_num_categories; i++){
				if (cat_split_set[i])
					str<<i<<" ";
			}
			str << "\\}$";
		}
		else
			str << "$f_{" << feature_index << "}<=" << num_split_value << "$";
		return(str.str());
	}
	
	index_t get_feature_index() const {return(feature_index);}
	num_t get_num_split_value() const {return(num_split_value);}
	std::bitset<max_num_categories> get_cat_split_set() {return(cat_split_set);}
	
	/* \brief takes a subspace and returns the 2 corresponding subspaces after the split is applied
	 */
	std::array<std::vector< std::vector<num_t> >, 2> compute_subspaces( std::vector< std::vector<num_t> > &subspace){
		
	
		std::array<std::vector<std::vector<num_t> >, 2> subspaces = {subspace, subspace};

		// if feature is numerical
		if (! std::isnan(num_split_value)){
			// for the left child, the split value is the new upper bound
			subspaces[0][feature_index][1] = num_split_value;
			// for the right child the split value is the new lower bound
			subspaces[1][feature_index][0] = num_split_value;
		}
		else{
			std::cout<<"updating categorical parameter\n";
			// every element in the split set should go to the left -> remove from right
			auto it = std::partition (subspaces[0][feature_index].begin(), subspaces[0][feature_index].end(),
										[this] (int i) {return((bool) this->cat_split_set[i]);});
		
			// replace the values in the 'right subspace'
			subspaces[1][feature_index].assign(it, subspaces[0][feature_index].end());
			
			// delete all values in the 'left subspace'
			subspaces[0][feature_index].resize(std::distance(subspaces[0][feature_index].begin(),it));
		}
		return(subspaces);
	}

	bool can_be_split(num_t *feature_vector){
		return(!std::isnan(feature_vector[feature_index]));
	}
};


}}//namespace rfr::splits
#endif
