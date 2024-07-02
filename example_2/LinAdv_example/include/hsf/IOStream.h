//
// 文件名:	IOStream.h
// 软件包:	JAUMIN toolbox
// 版权  :	(c) 2004-2010 北京应用物理与计算数学研究所
// 版本号:	$Revision: 257 $
// 修改  :	$Date: 2012-02-16 12:55:07 +0800 (四, 2012-02-16) $
// 描述  :	标准I/O数据流类
// 类别  :  %Internal File% ( Don't delete this line )
//

#ifndef included_Stream
#define included_Stream

#ifndef included_JAUMIN_config
#include "JAUMIN_config.h"
#endif

#ifndef included_stdio
#define included_stdio
#include <stdio.h>
#endif

#ifndef included_iostream
#define included_iostream
#include <iostream>
#endif

#ifndef included_iomanip
#define included_iomanip
#include <iomanip>
#endif

#ifndef LACKS_SSTREAM
#ifndef included_sstream
#define included_sstream
#include <sstream>
#endif
#endif

#ifndef LACKS_STRSTREAM
#ifndef included_strstream
#define included_strstream
#include <strstream>
#endif
#endif
using namespace std;

#endif
