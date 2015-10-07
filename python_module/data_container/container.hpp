#include "mostly_continuous.cpp"
#include "numpy_data_container.hpp"
#include "numpy_transposed_data_container.hpp"






void export_data_container(){
    namespace bp = boost::python;

    // map the regression namespace to a sub-module
    // make "from mypackage.Util import <whatever>" work
    bp::object dataModule(bp::handle<>(bp::borrowed(PyImport_AddModule("rfr.data_container"))));

    // make "from mypackage import Util" work
    bp::scope().attr("data_container") = dataModule;

    // set the current scope to the new sub-module
    bp::scope data_scope = dataModule;

    boost::python::class_<pyrfr::data_container::mostly_continuous_data <pyrfr_num_type, pyrfr_response_type_regression, pyrfr_index_type> >("mostly_continuous_data_regression", boost::python::init<pyrfr_index_type>())
	// add methods
	// to do: add a method to return numpy arrays with the data so far
	.def("add_data_point",
		&pyrfr::data_container::mostly_continuous_data<pyrfr_num_type, pyrfr_response_type_regression, pyrfr_index_type>::add_data_point_numpy)
	.def("set_type_of_feature",
		&pyrfr::data_container::mostly_continuous_data<pyrfr_num_type, pyrfr_response_type_regression, pyrfr_index_type>::set_type_of_feature);


	boost::python::class_<pyrfr::data_container::numpy_data_container<pyrfr_num_type, pyrfr_response_type_regression, pyrfr_index_type> >("numpy_data_container_regression", boost::python::init<boost::numpy::ndarray, boost::numpy::ndarray, boost::numpy::ndarray> ());
	boost::python::class_<pyrfr::data_container::numpy_transposed_data_container<pyrfr_num_type, pyrfr_response_type_regression, pyrfr_index_type> >("numpy_transposed_data_container_regression", boost::python::init<boost::numpy::ndarray, boost::numpy::ndarray, boost::numpy::ndarray> ());
	
}
