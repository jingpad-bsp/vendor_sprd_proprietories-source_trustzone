#ifndef _SPRD_PUBEFUSE_HEADER_H_
#define _SPRD_PUBEFUSE_HEADER_H_

#if defined(CONFIG_SOC_SHARKL3)
#include "sprd_pubefuse_sharkl3.h"
#elif defined(CONFIG_SOC_SHARKLE)
#include "sprd_pubefuse_sharkle.h"
#elif defined(CONFIG_SOC_PIKE2)
#include "sprd_pubefuse_pike2.h"
#else
#include "sprd_pubefuse_default.h"
#endif

#endif
