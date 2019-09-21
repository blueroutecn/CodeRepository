/*-----------------------------------------------------------
texts.h -- SwmmToGrms程序中错误和警告等提示信息。

-------------------------------------------------------------*/



/*--- 头文件 ------------------------------------------------*/
#pragma once
#include<vector>
#include<string>



/*--- 命名空间 ---------------------------------------------*/
using std::vector;
using std::string;



/*--- SwmmToGrms命名空间错误和警告信息 ---------------------*/
namespace SwmmToGrms
{
	/*--- log提示信息 --------------------------------------*/
	const string INFO1 = 
	"---------  ---------  ---------  ---------  ---------  --------- o SwmmToGrms o ";
	const string INFO2  = "INFO:  swmm input file(%s)";
	const string INFO3  = "INFO:  swmm special nodes counted: ";
	const string INFO4  = "INFO:  swmm special pipes counted: ";
	const string INFO5  = "INFO:  swmm special sections counted: ";
	const string INFO6  = "INFO:  swmm node counted: %d";
	const string INFO7  = "INFO:  swmm pipe counted: %d";
	const string INFO8  = "INFO:  swmm time series counted: %d";
	const string INFO9  = "INFO:  swmm transection counted: %d";
	const string INFO10 = "INFO:  swmm option accquired";
	const string INFO11 = "INFO:  grms hd counted: %d";
	const string INFO12 = "INFO:  grms junction counted: %d";
	const string INFO13 = "INFO:  grms bcsection counted: %d";
	const string INFO14 = "INFO:  grms location counted: %d";
	const string INFO15 = "INFO:  grms hd counted: %d";
	const string INFO16 = "INFO:  grms manning single counted: %d";
	const string INFO17 = "INFO:  all swmm time series have same length.";
	const string INFO18 = "INFO:  grms bctimeseries converted,counted(%d).";
	const string INFO19 = "INFO:  grms dst converted,counted(%d).";
	const string INFO20 = "INFO:  grms set converted.";
	const string INFO21 = "INFO:  grms pond recognized,counted(%d).";
	const string INFO22 = "INFO:  grms hd.inp writed.";
	const string INFO23 = "INFO:  grms dst.inp writed.";
	const string INFO24 = "INFO:  grms junction.inp writed.";
	const string INFO25 = "INFO:  grms crosssectionlocation.inp writed.";
	const string INFO26 = "INFO:  grms setting.inp writed.";
	const string INFO27 = "INFO:  grms manning.inp writed.";
	const string INFO28 = "INFO:  grms psource converted,counted(%d).";
	const string INFO29 = "INFO:  grms psource.inp writed.";
	const string INFO30 = "INFO:  grms bccrosssection.inp writed.";
	const string INFO31 = "INFO:  grms bctimeseries.inp writed.";


	const string WARN1  = "WARN:  swmm time series(%s) is unsupported yet.";
	const string WARN2  = "WARN:  swmm conduit with un-IRREGULAR transect: ";
	const string WARN3  = "WARN:  swmm un-IRREGULAR transect:";
	const string WARN4  = "WARN:  swmm timeserie(%s) is date-time-value formated";
	const string WARN5  = "WARN:  node(%s) is not conected.";
	const string WARN6  = "WARN:  swmm time series have different lengths.";
	const string WARN7  = "WARN:  grms bcsluice recognized,counted(%d).";
	const string WARN8  = "WARN:  grms sluice recognized,counted(%d).";
	const string WARN9  = "WARN:  grms bcweir recognized,counted(%d).";
	const string WARN10 = "WARN:  grms weir recognized,counted(%d).";
	const string WARN11 = "WARN:  swmm pump(%s) don't have curve.";
	const string WARN12 = "WARN:  gr pump(%s),its dm(%s) may have exception.";
	const string WARN13 = "WARN:  gr pump recognized,counted(%d).";
	const string WARN14 = "WARN:  gr psource(%s),its dm(%s) may have exception.";
	const string WARN15 = "WARN:  gr bcsluices.inp writed, lack infomation.";
	const string WARN16 = "WARN:  gr sluices.inp writed, lack infomation.";
	const string WARN17 = "WARN:  gr bcweirs.inp writed, lack infomation.";
	const string WARN18 = "WARN:  gr weirs.inp writed, lack infomation.";
	const string WARN19 = "WARN:  gr pumps.inp writed, lack infomation.";
	const string WARN20 = "WARN:  gr ponds.inp writed, lack infomation.";


	const string ERR1   = "ERRO:  swmm input file unfound";
	const string ERR2   = "ERRO:  element(%s) unfound";
	const string ERR3   = "ERRO:  time serie(%s) fomate mistake";
	const string ERR4   = "ERRO:  swmm conduit(%s) section unsupported";
	const string ERR5   = "ERRO:  swmm node(%s) connection exception";
	const string ERR6   = "ERRO:  swmm pipe(%s) can't acquire a transect";
	const string ERR7   = "ERRO:  swmm node(grms dm, %s) don't have a transect";
	const string ERR8   = "ERRO:  reach(%d) can't find a transect";
	const string ERR9   = "ERRO:  dm(%s) can't be found in swmm-transects.";
	const string ERR10  = "ERRO:  dm(%s) roughness exception.";
	const string ERR11  = "ERRO:  datatime(%s) exception.";
	const string ERR12  = "ERRO:  setconvertor need simulation happened during one month.";
	const string ERR13  = "ERRO:  gr pond(%s,type=1), can't match gr junction.";
	const string ERR14  = "ERRO:  gr folder(%s) is not a valid directory.";
}






