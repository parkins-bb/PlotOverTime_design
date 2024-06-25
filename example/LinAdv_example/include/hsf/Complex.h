// 文件名:	Complex.h
// 软件包:	JAUMIN toolbox
// 版权  :	(c) 2004-2010 北京应用物理与计算数学研究所
// 版本号:	$Revision: 257 $
// 修改  :	$Date: 2012-02-16 12:55:07 +0800 (四, 2012-02-16) $
// 描述  :	 dcomplex 类, 定义旧风格的compex和新风格的compex<double>.
//

#ifndef included_tbox_Complex
#define included_tbox_Complex

#ifndef included_JAUMIN_config
#include "JAUMIN_config.h"
#endif

#ifndef included_complex
#include <complex>
#define included_complex
using namespace std;
#endif

#ifndef LACKS_TEMPLATE_COMPLEX
typedef complex<double> dcomplex;
#else
typedef complex dcomplex;
#endif

#endif
