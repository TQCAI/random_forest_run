#include <boost/test/unit_test.hpp>

#include <numeric>
#include <cstring>


#include "rfr/data_containers/mostly_continuous_data_container.hpp"
#include "rfr/data_containers/mostly_continuous_data_with_instances_container.hpp"

typedef double num_t;
typedef double response_type;
typedef unsigned int index_type;
typedef std::default_random_engine rng_type;

typedef rfr::data_containers::mostly_continuous_data<num_t, response_type, index_type> data_container_type;
typedef rfr::data_containers::mostly_continuous_data_with_instances<num_t, response_type, index_type> data_container_type2;


BOOST_AUTO_TEST_CASE( data_container_tests ){
	data_container_type data;

	char *filename = (char*) malloc(1024*sizeof(char));


	strcpy(filename, boost::unit_test::framework::master_test_suite().argv[1]);
	strcat(filename, "diabetes_features.csv");
	data.read_feature_file(filename);

	strcpy(filename, boost::unit_test::framework::master_test_suite().argv[1]);
	strcat(filename, "diabetes_responses.csv");
	data.read_response_file(filename);

	// reference directly from scikit learn (from where the dataset was retrieved)
	std::vector<num_t> responses_reference {	151.,75.,141.,206.,135.,97.,138.,63.,110.,310.,101.,69.,179.,185.,118.,171.,166.,144.,97.,168.,68.,49.,68.,245.,184.,202.,137.,85.,131.,283.,129.,59.,341.,87.,65.,102.,265.,276.,252.,90.,100.,55.,61.,92.,259.,53.,190.,142.,75.,142.,155.,225.,59.,104.,182.,128.,52.,37.,170.,170.,61.,144.,52.,128.,71.,163.,150.,97.,160.,178.,48.,270.,202.,111.,85.,42.,170.,200.,252.,113.,143.,51.,52.,210.,65.,141.,55.,134.,42.,111.,98.,164.,48.,96.,90.,162.,150.,279.,92.,83.,128.,102.,302.,198.,95.,53.,134.,144.,232.,81.,104.,59.,246.,297.,258.,229.,275.,281.,179.,200.,200.,173.,180.,84.,121.,161.,99.,109.,115.,268.,274.,158.,107.,83.,103.,272.,85.,280.,336.,281.,118.,317.,235.,60.,174.,259.,178.,128.,96.,126.,288.,88.,292.,71.,197.,186.,25.,84.,96.,195.,53.,217.,172.,131.,214.,59.,70.,220.,268.,152.,47.,74.,295.,101.,151.,127.,237.,225.,81.,151.,107.,64.,138.,185.,265.,101.,137.,143.,141.,79.,292.,178.,91.,116.,86.,122.,72.,129.,142.,90.,158.,39.,196.,222.,277.,99.,196.,202.,155.,77.,191.,70.,73.,49.,65.,263.,248.,296.,214.,185.,78.,93.,252.,150.,77.,208.,77.,108.,160.,53.,220.,154.,259.,90.,246.,124.,67.,72.,257.,262.,275.,177.,71.,47.,187.,125.,78.,51.,258.,215.,303.,243.,91.,150.,310.,153.,346.,63.,89.,50.,39.,103.,308.,116.,145.,74.,45.,115.,264.,87.,202.,127.,182.,241.,66.,94.,283.,64.,102.,200.,265.,94.,230.,181.,156.,233.,60.,219.,80.,68.,332.,248.,84.,200.,55.,85.,89.,31.,129.,83.,275.,65.,198.,236.,253.,124.,44.,172.,114.,142.,109.,180.,144.,163.,147.,97.,220.,190.,109.,191.,122.,230.,242.,248.,249.,192.,131.,237.,78.,135.,244.,199.,270.,164.,72.,96.,306.,91.,214.,95.,216.,263.,178.,113.,200.,139.,139.,88.,148.,88.,243.,71.,77.,109.,272.,60.,54.,221.,90.,311.,281.,182.,321.,58.,262.,206.,233.,242.,123.,167.,63.,197.,71.,168.,140.,217.,121.,235.,245.,40.,52.,104.,132.,88.,69.,219.,72.,201.,110.,51.,277.,63.,118.,69.,273.,258.,43.,198.,242.,232.,175.,93.,168.,275.,293.,281.,72.,140.,189.,181.,209.,136.,261.,113.,	131.,174.,257.,55.,84.,42.,146.,212.,233.,91.,111.,152.,120.,67.,310.,94.,183.,66.,173.,72.,49.,64.,48.,178.,104.,132.,220.,57.};
	std::vector<num_t> feature_zero_reference {0.0380759064334241018,-0.00188201652779104007,0.08529890629667830071,-0.08906293935226029801,0.00538306037424807031,-0.09269547780327989928,-0.04547247794002570037,0.06350367559056099842,0.04170844488444359899,-0.07090024709716259699,-0.09632801625429950054,0.02717829108036539862,0.0162806757273066989,0.00538306037424807031,0.04534098333546320025,-0.05273755484206479882,-0.00551455497881059038,0.07076875249260000666,-0.03820740103798660192,-0.02730978568492789874,-0.04910501639104519755,-0.08543040090124079389,-0.08543040090124079389,0.04534098333546320025,-0.06363517019512339445,-0.06726770864614299572,-0.10722563160735799903,-0.02367724723390840155,0.0526060602375022987,0.06713621404158050254,-0.06000263174410389727,-0.02367724723390840155,0.03444336798240450054,0.03081082953138499989,0.0162806757273066989,0.04897352178648269744,0.01264813727628719998,-0.00914709342983014047,-0.00188201652779104007,-0.00188201652779104007,0.00538306037424807031,-0.09996055470531900466,-0.06000263174410389727,0.01991321417832630017,0.04534098333546320025,0.02717829108036539862,-0.05637009329308430294,-0.07816532399920170238,0.06713621404158050254,-0.0418399394890060991,0.03444336798240450054,0.05987113713954139715,-0.05273755484206479882,-0.00914709342983014047,-0.04910501639104519755,-0.0418399394890060991,-0.0418399394890060991,-0.02730978568492789874,0.04170844488444359899,0.06350367559056099842,-0.07090024709716259699,-0.0418399394890060991,-0.02730978568492789874,-0.03457486258696700065,0.06713621404158050254,-0.04547247794002570037,-0.00914709342983014047,0.04170844488444359899,0.0380759064334241018,0.0162806757273066989,-0.00188201652779104007,-0.00188201652779104007,0.06350367559056099842,0.01264813727628719998,0.01264813727628719998,-0.00914709342983014047,-0.0309423241359475,-0.09632801625429950054,0.00538306037424807031,-0.10359309315633899451,0.07076875249260000666,0.01264813727628719998,-0.01641217033186929963,-0.03820740103798660192,0.00175052192322852,0.04534098333546320025,-0.07090024709716259699,0.04534098333546320025,-0.05273755484206479882,-0.04547247794002570037,0.01264813727628719998,0.04534098333546320025,-0.02004470878288880029,-0.04910501639104519755,-0.07816532399920170238,-0.07090024709716259699,0.05623859868852180283,-0.02730978568492789874,0.00175052192322852,-0.00188201652779104007,0.0162806757273066989,0.0162806757273066989,-0.09269547780327989928,0.05987113713954139715,-0.02730978568492789874,0.02354575262934580082,-0.09632801625429950054,0.02717829108036539862,0.01991321417832630017,0.0380759064334241018,0.04170844488444359899,0.01991321417832630017,-0.08543040090124079389,0.01991321417832630017,0.02354575262934580082,-0.0309423241359475,0.04897352178648269744,0.05987113713954139715,-0.05637009329308430294,0.0162806757273066989,-0.04910501639104519755,0.06350367559056099842,0.04897352178648269744,0.00538306037424807031,-0.00551455497881059038,-0.00551455497881059038,-0.08906293935226029801,0.03444336798240450054,-0.05273755484206479882,0.00901559882526762994,-0.06363517019512339445,-0.09632801625429950054,0.0162806757273066989,-0.0418399394890060991,-0.07453278554818210111,-0.00551455497881059038,-0.09269547780327989928,0.00538306037424807031,0.03444336798240450054,0.02354575262934580082,0.04170844488444359899,-0.02730978568492789874,0.04170844488444359899,-0.0309423241359475,0.03081082953138499989,-0.0418399394890060991,-0.0309423241359475,-0.05637009329308430294,-0.06000263174410389727,-0.04910501639104519755,0.02354575262934580082,0.00175052192322852,-0.03457486258696700065,0.04170844488444359899,0.06713621404158050254,-0.02730978568492789874,-0.01641217033186929963,-0.00188201652779104007,-0.0127796318808497001,-0.00551455497881059038,-0.00914709342983014047,-0.04547247794002570037,-0.05273755484206479882,0.0162806757273066989,0.04534098333546320025,-0.0418399394890060991,-0.05637009329308430294,0.07076875249260000666,0.00175052192322852,-0.00188201652779104007,0.02354575262934580082,-0.02004470878288880029,0.04170844488444359899,-0.06363517019512339445,0.0162806757273066989,0.06713621404158050254,0.04534098333546320025,0.04897352178648269744,0.04170844488444359899,-0.02367724723390840155,-0.03820740103798660192,0.04897352178648269744,0.04534098333546320025,0.04534098333546320025,0.0162806757273066989,-0.07453278554818210111,-0.0817978624502212065,-0.06726770864614299572,0.00538306037424807031,-0.00188201652779104007,0.00901559882526762994,-0.00551455497881059038,0.05623859868852180283,0.00901559882526762994,-0.06726770864614299572,0.02717829108036539862,-0.02367724723390840155,0.04897352178648269744,-0.05273755484206479882,0.04170844488444359899,0.05623859868852180283,-0.03457486258696700065,0.08166636784565869944,-0.00188201652779104007,0.1107266754538149961,-0.0309423241359475,0.00175052192322852,0.00901559882526762994,0.03081082953138499989,0.0380759064334241018,0.00901559882526762994,0.0925639831987174061,0.06713621404158050254,0.00175052192322852,0.03081082953138499989,0.02717829108036539862,0.01264813727628719998,0.07440129094361959405,-0.0418399394890060991,-0.08906293935226029801,0.02354575262934580082,-0.04547247794002570037,-0.02367724723390840155,-0.09996055470531900466,-0.02730978568492789874,0.03081082953138499989,-0.10359309315633899451,0.06713621404158050254,-0.05273755484206479882,-0.02730978568492789874,-0.03820740103798660192,0.00901559882526762994,0.01264813727628719998,0.06713621404158050254,0.04534098333546320025,0.06713621404158050254,0.02717829108036539862,0.05623859868852180283,0.03444336798240450054,0.02354575262934580082,0.04897352178648269744,0.03081082953138499989,-0.10359309315633899451,0.0162806757273066989,-0.06000263174410389727,-0.02730978568492789874,0.04170844488444359899,-0.0817978624502212065,-0.0418399394890060991,-0.0127796318808497001,0.06713621404158050254,-0.05273755484206479882,0.00538306037424807031,0.08166636784565869944,0.03081082953138499989,0.00175052192322852,-0.04910501639104519755,-0.02730978568492789874,0.07803382939463919532,0.01264813727628719998,0.04170844488444359899,0.04897352178648269744,-0.01641217033186929963,-0.07453278554818210111,0.03444336798240450054,-0.03457486258696700065,-0.05273755484206479882,0.05987113713954139715,0.06350367559056099842,0.00901559882526762994,0.00538306037424807031,0.0380759064334241018,0.01264813727628719998,0.07440129094361959405,0.0162806757273066989,-0.00551455497881059038,0.01264813727628719998,-0.03457486258696700065,0.06713621404158050254,0.0380759064334241018,0.00901559882526762994,-0.09269547780327989928,0.07076875249260000666,-0.01641217033186929963,0.04170844488444359899,0.01264813727628719998,-0.03820740103798660192,0.04534098333546320025,0.07076875249260000666,-0.07453278554818210111,0.05987113713954139715,0.07440129094361959405,0.00901559882526762994,-0.07090024709716259699,0.02354575262934580082,-0.05273755484206479882,0.06713621404158050254,0.00175052192322852,0.02354575262934580082,0.0380759064334241018,0.0162806757273066989,-0.00188201652779104007,0.01264813727628719998,0.07440129094361959405,0.04170844488444359899,-0.00914709342983014047,0.00901559882526762994,0.06713621404158050254,0.00175052192322852,-0.00914709342983014047,-0.00551455497881059038,0.09619652164973699349,-0.07453278554818210111,0.05987113713954139715,-0.02367724723390840155,0.00901559882526762994,0.0162806757273066989,0.01991321417832630017,0.08893144474769780483,0.01991321417832630017,-0.02367724723390840155,0.09619652164973699349,0.02354575262934580082,0.07076875249260000666,0.03081082953138499989,-0.00188201652779104007,0.04534098333546320025,0.07440129094361959405,-0.03820740103798660192,-0.0127796318808497001,0.00901559882526762994,0.08166636784565869944,0.03081082953138499989,0.02717829108036539862,-0.06000263174410389727,0.00538306037424807031,-0.02004470878288880029,0.01991321417832630017,-0.06363517019512339445,0.02717829108036539862,-0.01641217033186929963,0.03081082953138499989,0.05623859868852180283,-0.02004470878288880029,-0.10722563160735799903,0.08166636784565869944,0.00538306037424807031,0.0380759064334241018,0.03081082953138499989,0.00175052192322852,-0.02730978568492789874,-0.08543040090124079389,0.01264813727628719998,-0.05273755484206479882,-0.02367724723390840155,-0.07453278554818210111,-0.00551455497881059038,-0.06000263174410389727,-0.02004470878288880029,0.0380759064334241018,0.0162806757273066989,0.04170844488444359899,0.01991321417832630017,-0.04910501639104519755,0.00175052192322852,0.03444336798240450054,-0.04547247794002570037,-0.00914709342983014047,-0.01641217033186929963,-0.00914709342983014047,0.01991321417832630017,0.0526060602375022987,-0.02730978568492789874,-0.07453278554818210111,-0.10722563160735799903,0.04534098333546320025,-0.00188201652779104007,0.01991321417832630017,0.0162806757273066989,-0.00188201652779104007,0.0162806757273066989,-0.07090024709716259699,0.04897352178648269744,0.00538306037424807031,0.03444336798240450054,0.02354575262934580082,0.01991321417832630017,-0.04547247794002570037,0.0526060602375022987,-0.00551455497881059038,0.00901559882526762994,-0.02367724723390840155,-0.0418399394890060991,-0.07453278554818210111,0.03444336798240450054,-0.06000263174410389727,-0.08543040090124079389,0.0526060602375022987,0.01264813727628719998,0.05987113713954139715,-0.02367724723390840155,0.0162806757273066989,0.1107266754538149961,-0.02004470878288880029,-0.01641217033186929963,0.04897352178648269744,-0.05637009329308430294,0.02717829108036539862,0.06350367559056099842,-0.05273755484206479882,-0.00914709342983014047,0.00538306037424807031,0.07440129094361959405,-0.05273755484206479882,0.08166636784565869944,-0.00551455497881059038,-0.02730978568492789874,-0.05273755484206479882,0.00901559882526762994,-0.02004470878288880029,0.02354575262934580082,0.0380759064334241018,-0.07816532399920170238,0.00901559882526762994,0.00175052192322852,-0.07816532399920170238,0.03081082953138499989,-0.03457486258696700065,0.04897352178648269744,-0.0418399394890060991,-0.00914709342983014047,0.07076875249260000666,0.00901559882526762994,-0.02730978568492789874,0.0162806757273066989,-0.0127796318808497001,-0.05637009329308430294,0.04170844488444359899,-0.00551455497881059038,0.04170844488444359899,-0.04547247794002570037,-0.04547247794002570037};
	std::vector<num_t> feature_five_reference {-3.4820762837698603e-02,-1.9163339748222000e-02,-3.4194465914119503e-02,2.4990593364102101e-02,1.5596139510416100e-02,-7.9287844411812206e-02,-2.4800012060433599e-02,1.0891438112369700e-01,6.2016856567301600e-03,-3.4507614375908997e-02,-9.0561189036235307e-02,4.5971540304000802e-02,-9.7688858945359901e-03,-1.5718706668537100e-02,-6.1283579060483295e-05,1.0766178727653899e-01,-2.3860566675064900e-02,4.9416173383685598e-02,-1.9476488210011501e-02,-1.1334628203483700e-02,-4.3275771306016002e-02,-2.6365754369381201e-02,7.7674279656778202e-03,-4.7346701309279898e-02,-1.8850191286432401e-02,4.6359433477824999e-03,-9.6197861348446906e-02,-4.3588919767805497e-02,-3.7639098993804403e-02,-9.5884712886657397e-02,-7.5768466620092798e-03,-5.3609670545070501e-02,-1.2900370512431300e-02,-8.9934892112656303e-02,-4.8912443618227497e-02,-4.1322135823244196e-03,-2.8557793601907900e-02,-4.2962622844226403e-02,7.5407495712216804e-02,6.2016856567301600e-03,7.3215456479689994e-02,-7.2711726714231997e-02,-2.3547418213275401e-02,-1.0082034356325500e-02,4.6359433477824999e-03,1.5044587298871800e-03,-4.7659849771069400e-02,-7.4277469023179707e-02,5.8885371949406297e-03,6.2016856567301600e-03,-3.7325950532014901e-02,-2.9184090525487000e-02,4.4718946456842601e-02,2.2172257207996302e-02,1.7788178742942799e-02,-2.2607972827906799e-02,-3.0436684372645101e-02,-1.0433972135497500e-01,-5.7993749010124003e-02,4.8476727998317003e-02,-7.8661547488233105e-02,-2.7305199754749800e-02,-1.3839815897799900e-02,4.3466352609684497e-02,-5.9872639780861200e-02,1.4969842586837100e-02,1.2151506430731301e-02,8.3937248892568799e-03,-2.4173715136854498e-02,-6.1438382089808799e-02,-9.1425889709569400e-03,2.6243187211260201e-02,4.8789876460106497e-02,5.3173954925160000e-02,1.7161881819363799e-02,-1.0007289644290900e-03,-1.0684490904929100e-01,-2.7618348216539301e-02,-6.8327648249178502e-02,1.9980217975469599e-02,5.3487103386949501e-02,2.8435226443786901e-02,1.7161881819363799e-02,-5.2983373621491497e-02,-3.0123535910855599e-02,-1.0007289644290900e-03,-1.5092409744957999e-02,1.6222436433995201e-02,-3.7012802070225302e-02,-1.6031855130326601e-02,-4.5154662076753199e-02,6.4760448011372707e-02,-2.0729082057169598e-02,-1.5405558206747600e-02,-1.3526667436010401e-02,-2.8870942063697499e-02,-1.2274073588852300e-02,4.2526907224315902e-02,-1.3839815897799900e-02,3.7516531835683403e-02,4.4405797995053099e-02,-3.7443204085001999e-04,2.4990593364102101e-02,7.0710268785373800e-02,-1.7284448977484799e-02,-2.1668527442538200e-02,-3.4820762837698603e-02,4.0648016453578698e-02,-5.5801709777597297e-02,3.7516531835683403e-02,-4.0144286688120603e-02,1.0272615659994100e-02,1.9667069513680000e-02,2.0293366437259101e-02,-3.2941872066961399e-02,4.9416173383685598e-02,2.0606514899048599e-02,3.1566711061682302e-02,4.0021719529999597e-02,-4.2962622844226403e-02,-1.8537042824642899e-02,2.1545960284417201e-02,6.4134151087793606e-02,1.9878798965729300e-01,-3.8891692840962500e-02,-6.3630421322335595e-02,-5.4549115930439103e-02,1.3717248739678899e-02,-5.0716589676930001e-03,1.0585764121783600e-02,-4.7585105059034698e-03,-1.0708331279904599e-02,4.7850431074737999e-02,-7.1772281328863402e-02,2.5186488272903101e-04,-4.2962622844226403e-02,-6.8014499787388993e-02,-1.6345003592116200e-02,-3.3881317452330001e-02,-4.8912443618227497e-02,-1.3138774262186300e-03,5.7244884928423903e-02,4.4405797995053099e-02,6.8279825803092102e-03,5.0668767230843799e-02,-3.2628723605171897e-02,3.1566711061682302e-02,2.3424851055154398e-02,4.8163579536527501e-02,-2.0415933595380100e-02,-1.4466112821378999e-02,-2.3860566675064900e-02,6.6952487243899406e-02,1.6222436433995201e-02,2.6243187211260201e-02,8.6367691874850397e-02,-3.5760208223067198e-02,4.2840055686105501e-02,1.6848733357574301e-02,7.1411310420987500e-03,-1.3213518974220901e-02,1.3146107037254301e-01,-1.1334628203483700e-02,5.5753887331510899e-03,-1.3526667436010401e-02,-4.3275771306016002e-02,-7.0832835943494807e-02,1.5044587298871800e-03,6.3194705702424997e-02,-1.6031855130326601e-02,-1.5092409744957999e-02,-5.9872639780861200e-02,1.5909287972205598e-02,-6.6448757478441395e-02,-9.4632119039499293e-02,-2.6365754369381201e-02,2.5930038749470700e-02,1.8414475666521900e-02,1.6222436433995201e-02,4.1274313377157798e-02,5.8885371949406297e-03,5.0042470307264698e-02,8.9186028030956196e-02,-1.6270258880081499e-03,-2.6052605907591699e-02,-2.4800012060433599e-02,5.7558033390213398e-02,-8.2106180567918005e-02,7.0710268785373800e-02,3.0702010388348401e-03,-5.0716589676930001e-03,-7.6156359793916897e-02,2.1232811822627699e-02,8.3862504180534203e-02,-4.9538740541806597e-02,4.8476727998317003e-02,3.6263937988525299e-02,1.3404100277889399e-02,-3.3881317452330001e-02,-1.5718706668537100e-02,4.9103024921896103e-02,-7.8899951237987893e-03,9.1064918801693401e-02,5.6618588004844900e-02,-7.2636982002197399e-03,-4.7033552847490299e-02,3.8455977221051998e-02,-1.3839815897799900e-02,3.7829680297472898e-02,-1.6658152053905698e-02,4.7224134151158899e-02,-1.6658152053905698e-02,3.1253562599892801e-02,-1.0007289644290900e-03,-8.1166735182549396e-02,-3.2002426681592797e-02,7.4154901865058701e-02,3.6890234912104303e-02,1.4343545663258000e-02,-7.4277469023179707e-02,-3.3255020528750900e-02,8.7816180630810505e-04,-4.7659849771069400e-02,-6.0185788242650702e-02,-4.1396880535278797e-02,-4.3588919767805497e-02,-2.4800012060433599e-02,-1.5718706668537100e-02,8.0805764274673401e-03,-6.8758050263955696e-04,1.5588665039212701e-01,8.7068733510464093e-03,5.7244884928423903e-02,-3.4507614375908997e-02,-2.4173715136854498e-02,-3.4507614375908997e-02,7.0710268785373800e-02,-1.0007289644290900e-03,-7.5768466620092798e-03,3.8769125682841500e-02,-2.2533228115872200e-03,2.7495781058418399e-02,-6.7701351325599499e-02,-1.6031855130326601e-02,-4.8912443618227497e-02,-5.8620045933702999e-02,5.0981915692633301e-02,-1.8537042824642899e-02,1.2801643729285800e-01,2.9374671829155499e-02,-3.9204841302752001e-02,4.8789876460106497e-02,5.2860806463370498e-02,5.6618588004844900e-02,-1.2274073588852300e-02,-1.9476488210011501e-02,-1.9789636671801000e-02,-2.3547418213275401e-02,5.2860806463370498e-02,1.2934377585205100e-02,-3.1376129758013703e-02,-2.5739457445802100e-02,1.7475030281153301e-02,-3.2628723605171897e-02,-9.7688858945359901e-03,-2.8796197351662902e-03,-3.6699653608435800e-02,7.9478425715480694e-02,3.7516531835683403e-02,-1.0395182818115099e-02,-4.7346701309279898e-02,-2.1042230518959201e-02,-6.6448757478441395e-02,3.2819304908840399e-02,-3.1927681969558101e-03,-1.5405558206747600e-02,9.1691215725272501e-02,-7.8035250564654005e-02,1.9667069513680000e-02,-2.6365754369381201e-02,1.0585764121783600e-02,-3.0436684372645101e-02,1.0296456034969600e-01,-3.6386505146646202e-02,-4.5154662076753199e-02,2.1232811822627699e-02,-8.6490259032971403e-02,1.2519810113675200e-01,5.6305439543055301e-02,5.3173954925160000e-02,1.6848733357574301e-02,4.2840055686105501e-02,-6.8640796710968094e-02,4.2526907224315902e-02,1.7788178742942799e-02,-1.2900370512431300e-02,-2.4486863598644000e-02,-2.4486863598644000e-02,-7.5530062870337797e-02,-2.1981675904327699e-02,-1.3526667436010401e-02,3.0000968752734600e-02,2.7182632596628800e-02,2.0293366437259101e-02,1.1642994420664600e-01,1.6222436433995201e-02,-2.4800012060433599e-02,3.5637641064946199e-02,-8.1479883644338905e-02,7.0083971861794700e-02,1.4969842586837100e-02,6.0689518008108803e-02,-1.4152964359589400e-02,1.9667069513680000e-02,-1.0007289644290900e-03,2.0606514899048599e-02,-2.1355378980748699e-02,7.4154901865058701e-02,8.7068733510464093e-03,-5.4549115930439103e-02,9.2004364187061996e-02,3.6577086450314801e-02,-3.6073356684856700e-02,-4.4453620441139499e-03,7.7912683406532998e-02,5.6618588004844900e-02,1.1913102680976399e-03,2.4990593364102101e-02,-2.3234269751485900e-02,-2.9184090525487000e-02,3.6890234912104303e-02,-4.6407255923911303e-02,-1.1334628203483700e-02,-1.6270258880081499e-03,-5.7680600548334501e-02,-6.6374012766406698e-03,8.9499176492745705e-02,7.1411310420987500e-03,2.4051147978733499e-02,5.6618588004844900e-02,-3.8190651205348802e-03,1.1913102680976399e-03,1.8101327204732402e-02,8.7068733510464093e-03,6.4447299549583198e-02,5.7871181852002997e-02,1.2739014036927901e-01,-1.2587222050641800e-02,-2.9497238987276499e-02,-4.7033552847490299e-02,-1.7597597439274301e-02,-6.9580242096336703e-02,-9.7137306733815501e-02,-3.1927681969558101e-03,8.8872879569166702e-02,-2.1668527442538200e-02,1.6222436433995201e-02,-3.0123535910855599e-02,-3.4194465914119503e-02,1.7475030281153301e-02,-1.3213518974220901e-02,-4.8286146694648500e-02,-2.6678902831170699e-02,-2.8244645140118398e-02,4.9490918095720197e-03,5.2622402713615500e-03,3.1879859523471797e-02,3.3758750294209001e-02,-1.0082034356325500e-02,-3.8190651205348802e-03,-1.2587222050641800e-02,-1.9163339748222000e-02,1.2777803354310301e-02,-5.7054303624755401e-02,-7.0519687481705298e-02,2.2485405669785900e-02,1.3020847652538500e-01,4.4718946456842601e-02,1.0898912583573100e-02,-1.1561306597939799e-01,-4.4215216691384500e-02,-2.5739457445802100e-02,-7.2711726714231997e-02,-2.8870942063697499e-02,8.6993988798429497e-02,-6.1125233628019297e-02,-1.7597597439274301e-02,-1.7284448977484799e-02,-5.0716589676930001e-03,-1.6658152053905698e-02,-2.3234269751485900e-02,-5.0478185927175200e-02,3.6577086450314801e-02,-2.3234269751485900e-02,-5.4549115930439103e-02,-2.1668527442538200e-02,-5.4235967468649601e-02,-5.8306897471913498e-02,3.2193007985261299e-02,1.1838357968941700e-02,-3.5447059761277599e-02,-5.3922819006860002e-02,2.0293366437259101e-02,-2.3860566675064900e-02,-2.7931496678328900e-02,-1.0089508827529001e-01,-3.7012802070225302e-02,3.9708571068210102e-02,1.9353921051890498e-02,7.2902308017900500e-02,3.7203383373893797e-02,-7.2398578252442503e-02,1.3090951816099899e-02,7.1411310420987500e-03,1.1705624113022500e-01,4.7224134151158899e-02,1.8101327204732402e-02,-1.3138774262186300e-03,9.4196403419588695e-02,-5.7367452086544903e-02,3.4698195679577597e-02,4.5032094918632103e-02,6.4447299549583198e-02,7.3528604941479600e-02,-3.8190651205348802e-03,-1.1279472982329200e-01,6.8831378014636596e-02,-6.2690975936967000e-02,2.1545960284417201e-02,4.1587461838947300e-02,4.5345243380421701e-02,-4.7033552847490299e-02,4.4718946456842601e-02,1.9980217975469599e-02,1.0898912583573100e-02,4.6359433477824999e-03,-4.7033552847490299e-02,-2.5664712733767599e-03,7.9165277253691199e-02,-1.3839815897799900e-02,1.5282991048626600e-02,2.7808929520207901e-02};

        // see if the class things everything went smoothly!
	BOOST_CHECK(data.check_consistency() == true);

	// check the dimensions
	BOOST_CHECK_EQUAL(data.num_features(), 10);
	BOOST_CHECK_EQUAL(data.num_data_points(), 442);

	std::vector<int> indices(data.num_data_points());
	std::iota(indices.begin(), indices.end(), 0);

	// check the response values

	for (size_t i =0; i < responses_reference.size(); i++){
        BOOST_CHECK( data.response(i) == responses_reference[i] );
	}

	// check the values of the first feature (index zero, of course :))
	std::vector<num_t> data_point_zero = data.retrieve_data_point(0);
	std::vector<num_t> data_point_zero_reference = {0.038075906433424102,  0.050680118739818703,  0.061696206518688498, 0.021872354994955798, -0.044223498424446402, -0.034820762837698603, -0.043400845652026898, -0.00259226199818282 ,  0.019908420876318299, -0.017646125159805199};

	BOOST_CHECK_EQUAL_COLLECTIONS ( data_point_zero_reference.begin(), data_point_zero_reference.end(),
					  data_point_zero.begin(), data_point_zero.end());

	for (size_t i =0; i < responses_reference.size(); i++){
        BOOST_CHECK( data.feature(0,i) == feature_zero_reference[i] );
	}

	// check the values of the sixth feature (index 5, of course :))
	std::vector<num_t> data_point_five = data.retrieve_data_point(5);
	std::vector<num_t> data_point_five_reference = {-0.0926954778032799, -0.044641636506989 , -0.0406959404999971, -0.0194420933298793, -0.0689906498720667, -0.0792878444118122, 0.0412768238419757, -0.076394503750001 , -0.0411803851880079, -0.0963461565416647};
	BOOST_CHECK_EQUAL_COLLECTIONS ( data_point_five_reference.begin(), data_point_five_reference.end(),
					  data_point_five.begin(), data_point_five.end());

	for (size_t i =0; i < responses_reference.size(); i++){
        BOOST_CHECK( data.feature(5,i) == feature_five_reference[i] );
	}

	// check that setting and retrieving the feature type works
	BOOST_CHECK( data.get_type_of_feature(0) == 0 );
	BOOST_CHECK_THROW( data.set_type_of_feature(1,10), std::runtime_error);

    free(filename);
}


BOOST_AUTO_TEST_CASE( data_container_with_instances_tests ){

	data_container_type2 data(2,2);

	data.set_type_of_configuration_feature(0,1);
	BOOST_CHECK(data.get_type_of_feature(0) == 1);

	data.set_type_of_instance_feature(0,2);
	BOOST_CHECK(data.get_type_of_feature(2) == 2);

	data.set_type_of_feature(0,0);
	data.set_type_of_instance_feature(0,0);

	num_t config1 [] = {1,1};
	num_t config2 [] = {2,2};
	num_t config3 [] = {3,3};

	num_t instance1 [] = {4,4};
	num_t instance2 [] = {5,5};

	BOOST_CHECK( data.add_configuration(config1, 2) == 0);
	BOOST_CHECK( data.add_configuration(config2, 2) == 1);
	BOOST_CHECK( data.add_configuration(config3, 2) == 2);
	BOOST_CHECK( data.num_configurations() == 3);

	BOOST_CHECK( data.add_instance(instance1,2) == 0);
	BOOST_CHECK( data.add_instance(instance2,2) == 1);
	BOOST_CHECK( data.num_instances() == 2);

	data.add_data_point(0u,0,0.1); // first argument cannot be "0" because than the compiler has trouble resolving the overloading!
	data.add_data_point(0u,1,0.2); // b/c "0" is also a valid pointer to a double!
	data.add_data_point(1,0,0.3);
	data.add_data_point(1,1,0.4);
	data.add_data_point(2,0,0.5);
	data.add_data_point(2,1,0.6);

	BOOST_CHECK (data.num_data_points() == 6);

	BOOST_CHECK_THROW(data.add_data_point(3,0,0.7), std::runtime_error);
	BOOST_CHECK_THROW(data.add_data_point(0u,3,0.8), std::runtime_error);

	std::vector<num_t> features0a = {1,1,4,4};
	std::vector<num_t> features0b = data.retrieve_data_point(0);
	
	BOOST_CHECK_EQUAL_COLLECTIONS(features0a.begin(), features0a.end(),
								  features0b.begin(), features0b.end());

	std::vector<num_t> features1a = {1,1,5,5};
	std::vector<num_t> features1b = data.retrieve_data_point(1);
	
	BOOST_CHECK_EQUAL_COLLECTIONS(features1a.begin(), features1a.end(),
								  features1b.begin(), features1b.end());

	std::vector<num_t> features2a = {2,2,4,4};
	std::vector<num_t> features2b = data.retrieve_data_point(2);
	
	BOOST_CHECK_EQUAL_COLLECTIONS(features2a.begin(), features2a.end(),
								  features2b.begin(), features2b.end());

	std::vector<num_t> features3a = {2,2,5,5};
	std::vector<num_t> features3b = data.retrieve_data_point(3);
	
	BOOST_CHECK_EQUAL_COLLECTIONS(features3a.begin(), features3a.end(),
								  features3b.begin(), features3b.end());

	std::vector<num_t> features4a = {3,3,4,4};
	std::vector<num_t> features4b = data.retrieve_data_point(4);
	
	BOOST_CHECK_EQUAL_COLLECTIONS(features4a.begin(), features4a.end(),
								  features4b.begin(), features4b.end());

	std::vector<num_t> features5a = {3,3,5,5};
	std::vector<num_t> features5b = data.retrieve_data_point(5);
	
	BOOST_CHECK_EQUAL_COLLECTIONS(features5a.begin(), features5a.end(),
								  features5b.begin(), features5b.end());

}
