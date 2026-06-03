#include "fibo_opencpu.h"

typedef enum {
SC_SIM_RETURN_SUCCESS,
SC_SIM_RETURN_FAIL,
SC_SIM_RTEURN_UNKNOW
}SC_simcard_err_e;

int sKDWL_NetworkGetCgreg(int* pGreg);
SC_simcard_err_e sKDWL_SysGetIccid(char *IccidValue);
//CUST_DEMO,END

