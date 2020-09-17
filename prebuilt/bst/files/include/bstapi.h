#ifndef bstapi_h__
#define bstapi_h__

#include <inttypes.h>

#define Array_Size 32

#define DEBUG_BST 1


struct bstresults
{
    uint32_t *uplinkresult;
    uint32_t *downlinkresult;
    uint32_t *latencyresult;
    uint32_t *serveridresult;
    char *locationresult;
    char *ISPresult;
};


extern int update_admin_user_settings(char* newadminsettings, char* newusersettings, char* overwrite);
extern int bstResults_Update(uint32_t uplink,uint32_t downlink,uint32_t latency, char* location,uint32_t serverid, char* ISP);
extern int bstResults_get(struct bstresults results);
extern int bst_Init(void);
extern int bst_Uninit(void);
extern int bst_Stop(void);
enum error_code
{
	ERROR_OK = 0,
	ERROR_NOK = 1,
	ERROR_INIT = -1,
	ERROR_INVALID_INPUT = -2,
        ERROR_INCOMPLETE_RESULTS = -3,
        ERROR_SPEEDTEST_IN_PROGRESS = -4,
        ERROR_SPEEDTEST_NOT_STARTED = -5,
};
#endif  // bstapi_h_
