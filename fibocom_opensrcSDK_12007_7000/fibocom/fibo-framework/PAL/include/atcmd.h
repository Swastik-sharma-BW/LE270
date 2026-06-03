#ifndef BC6FC25D_D194_451E_A25F_B09A76C59CF4
#define BC6FC25D_D194_451E_A25F_B09A76C59CF4

#include "cmsis_os2.h"
#include "at_util.h"
#include "atc_decoder.h"
#include "cmicomm.h"

CmsRetId  atDemo1(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtset(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_mipcall(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipopen(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipodm(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipsend(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mippush(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipread(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipclose(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipflush(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_miping(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_msdns(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_msdnsv6(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipntp(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipdns(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_sets(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_dsets(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipcall(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipconf(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipkeepconf(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipdcnt(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mipsendfile(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_pdntimerinfo(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_gtdns(AtCmdInputContext *pAtCmdReq);
#ifdef CONFIG_CUST_QIYUE_FEATURE
CmsRetId atcmd_mipcfg(AtCmdInputContext *pAtCmdReq);
#endif
#ifdef CONFIG_FIBOCOM_CHANGCHENG
CmsRetId  atcmd_push(AtCmdInputContext *pAtCmdReq);
#endif
CmsRetId  atcmd_loglevel(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gpio(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtmlv(AtCmdInputContext *pAtCmdReq);
#ifdef CONFIG_FFW_FEATURE_SUPPORT_AT_SIMPHASE
CmsRetId  atcmd_simphase(AtCmdInputContext *pAtCmdReq);
#endif
CmsRetId  atcmd_tgpio(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtdlmode(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_mmad(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_cbc(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_mtsm(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtlmv(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_cpwroff(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_httpset(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_httpact(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_httphead(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_httpread(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_httpdata(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_httpstop(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtota(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtfopen(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtfclose(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtfread(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtfwrite(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtfseek(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtfsize(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtfdelete(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtflist(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtfrename(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtfdlf(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_fileload(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mqttuser(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mqttopen(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mqttwill(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mqttpub(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mqttsub(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mqttunsub(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mqttclose(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mqttread(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mqttconf(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_mqttcfg(AtCmdInputContext *pAtCmdReq);
#ifdef CONFIG_CUST_SHANGJI_FEATURE
CmsRetId atcmd_mqttwakeupset(AtCmdInputContext *pAtCmdReq);
#endif
CmsRetId atcmd_ssl_GTSSLFILE(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_ssl_GTSSLMODE(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_ssl_GTSSLERR(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_ssl_GTSSLVER(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_ssl_GTSSLCIPHER(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_gtkey(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_gtgis(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_update(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_gtudset(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_gtud(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gthben(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gthbeat(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gthbtime(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtbuildhbtlink(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtlogin(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtdeshbtlink(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtstarthbt(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtwakeupcode(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_tuokuohb(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_ipr(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_cbaud(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_ifc(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_icf(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_andC(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_andD(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_andK(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unikeyinfom(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unidelkeyinfom(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unikeyinfo(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unidelkeyinfo(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unicertinfo(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unishcertinfo(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unidelcertinfo(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unimqttcon(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unimqttdiscon(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unimqttstate(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unimqttsub(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unimqttpub(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unimqttbindsub(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unimqttbind(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unimqttshadowsub(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unimqttshadowget(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unimqttshadowpub(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_unipsmset(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qicsgp(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qiact(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qideact(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qiopen(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qiclose(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qistate(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qisend(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qird(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qisendex(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qping(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qntp(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qidnscfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qidnsgip(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qicfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qcfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qisde(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qsslcfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qsslopen(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qsslsend(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qsslrecv(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qsslclose(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qisendcrc(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qurccfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qfupl(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qflst(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qfdel(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qfotadl(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtotadl(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtlst(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtrd(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_mippdncfg(AtCmdInputContext *pAtCmdReq);

CmsRetId atcmd_qfopen(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_qfclose(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_qfread(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_qfseek(AtCmdInputContext *pAtCmdReq);
// CmsRetId atcmd_qfdel(AtCmdInputContext *pAtCmdReq);

CmsRetId  atcmd_qftpopen(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qftpclose(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qftpcfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qftpget(AtCmdInputContext *pAtCmdReq);

CmsRetId  atcmd_secbooten(AtCmdInputContext *pAtCmdReq);

#ifdef CONFIG_FFW_WIFISCAN_ENABLE
CmsRetId atcmd_getapinfo(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_gtwifiscan(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FFW_LPM_SUPPORT
CmsRetId atcmd_s24(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_gtlpmmode(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_csclk(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_slpmode(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_gtwake(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_gtpmtime(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_wrim(AtCmdInputContext *pAtCmdReq);

#ifdef FFW_HAIKANG_CONFIG
CmsRetId atcmd_gtusbsleepen(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FFW_EC200_AT_ENABLE
CmsRetId atcmd_qsclk(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_qpowd(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FFW_AUDIO_ENABLE
#ifdef CONFIG_FFW_AUDIO_AT_SUPPORT
#ifdef FFW_NEWLAND_CONFIG
CmsRetId  atcmd_qaudplay(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qaudstop(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qaudplayspl(AtCmdInputContext *pAtCmdReq);
#endif
CmsRetId  atcmd_audrec(AtCmdInputContext *pAtCmdReq);
#endif
#if defined (CONFIG_FFW_AUDIO_AT_SUPPORT) || defined (CONFIG_FFW_TTS_AT_SUPPORT)
CmsRetId  atcmd_clvl(AtCmdInputContext *pAtCmdReq);
#endif
CmsRetId  atcmd_audplay(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_audlbtest(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef FFW_TRANSN_CONFIG
CmsRetId atcmd_seauthsta_transn(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_serand_transn(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_seformat_transn(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_seauthkey_transn(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_seauth_transn(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_serstpkey_transn(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_sesign_transn(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_secalauth_transn(AtCmdInputContext *pAtCmdReq);
#endif
#endif

#ifdef CONFIG_FFW_MQTT_CLOUD_ENABLE
CmsRetId  atcmd_mqttauthdyn(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_mqttauthcfg(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FFW_FTP_AT_ENABLE
CmsRetId  atcmd_ftpopen(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpclose(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpinfo(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpcwd(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpmkd(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftprmd(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftppwd(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpcdup(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpdel(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpren(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftplist(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpstat(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpsyst(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpnoop(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpstor(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpretr(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftprest(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpget(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftprecv(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpput(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpmode(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftpsize(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ftptime(AtCmdInputContext *pAtCmdReq);
#endif

CmsRetId atcmd_gtbdid(AtCmdInputContext *pAtCmdReq);
CmsRetId atcmd_gtfmode(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtrejcause(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtrrcrelcfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtjdc(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtdrxtm(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtsetrrm(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtchipinfo(AtCmdInputContext *pAtCmdReq);
#ifdef FFW_HAIKANG_CONFIG
CmsRetId  atcmd_gtcellinfo(AtCmdInputContext *pAtCmdReq);
#endif
CmsRetId  atcmd_gtcellscan(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtccinfo(AtCmdInputContext *pAtCmdReq);
// Add non-signaling AT support - 2025-08-07, JIRA:MTC0991-1000
CmsRetId  atcmd_gtnsmode(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtcb(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtnsrxlevel(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtnstxcfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtnsrxcfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtnstxstart(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtnstxstop(AtCmdInputContext *pAtCmdReq);
// Add non-signaling AT support - 2025-08-07, JIRA:MTC0991-1000





#ifdef CONFIG_FFW_TTS_AT_SUPPORT
#ifdef CONFIG_FFW_EC200_AT_ENABLE
CmsRetId  atcmd_qtts(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qttsetup(AtCmdInputContext *pAtCmdReq);
#else
CmsRetId  atcmd_gtts(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gttsspeed(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gttspitch(AtCmdInputContext *pAtCmdReq);
#endif
#endif
#ifdef CONFIG_FFW_ANDLINK_ENABLE
CmsRetId  atcmd_gtalconf(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtaldata(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtalact(AtCmdInputContext *pAtCmdReq);
#endif
#ifdef CONFIG_FFW_DM_ENABLE
CmsRetId  atcmd_gtdmconf(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FFW_EC200_MQTT_AT_ENABLE
CmsRetId  atcmd_QMTOPEN(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_QMTCONN(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_QMTDISC(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_QMTCLOSE(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_QMTSUB(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_QMTUNS(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_QMTPUBEX(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_QMTRECV(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_QMTCFG(AtCmdInputContext *pAtCmdReq);

#endif

#ifdef CONFIG_FIBOCOM_AT_GNSS_SUPPORT
CmsRetId  atcmd_gtgpspower(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtgps(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtgpscfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtgpsnmeacfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtgpsnmeafreq(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtgpsepo(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FFW_EC200_GNSS_AT_ENABLE
CmsRetId  atcmd_qgps(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_qgpsloc(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FIBOCOM_EXTERNAL_GNSS_SUPPORT
CmsRetId  atcmd_gtgpsset(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtagpsserv(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtgnssoat(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtgnssver(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtgpsauto(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FIBOCOM_BLE_SUPPORT
// CmsRetId  atcmd_GTMAC(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBTEN(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEVER(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEPADD(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLERADD(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLESETNM(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEDISCON(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLESTAT(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEADVEN(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLESETADV(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEADVDATA(AtCmdInputContext *pAtCmdReq);
// CmsRetId  atcmd_GTBLERSPDATA(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLESDDATA(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEMTU(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEPRMTU(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEUUID(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLESLEEP(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBTTEST(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTECOTA(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLETEST(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEAUTH(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEUUIDATTR(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLESEND(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEREADRSP(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTBLEBONDCTRL(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FFW_ECLOUD_AT_ENABLE
CmsRetId  atcmd_GTLWCTOPEN(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTLWCTDEL(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTLWCTSEND(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTLWCTREAD(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTLWCTUPDATE(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_SZAF_PROJECT
CmsRetId  atcmd_AFVERIFY(AtCmdInputContext *pAtCmdReq);
#endif
#ifdef CONFIG_CTIOT_MQTT_SUPPORT
CmsRetId  atcmd_CTDIAGNOSE(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_CTDIAGNOSEUSR(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_CTMQTTPUB(AtCmdInputContext *pAtCmdReq);
#endif
#ifdef CONFIG_FFW_MAAS_ENABLE
CmsRetId  atcmd_MAASREGSTATUS(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_MAASDEVREG(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_MAASCONNSTATE(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_MAASPUB(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_MAASCONFIG(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_MAASSDKVERSION(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_MAASCHANNEL(AtCmdInputContext *pAtCmdReq);
//CmsRetId  atcmd_MAASDEBUG(AtCmdInputContext *pAtCmdReq);
#endif
#ifdef CONFIG_FIBOCOM_XIHUANG
CmsRetId  atcmd_XHCGMM(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_XHCGMI(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_XHATI(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_XHEN(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_CUST_TUOKUO_FEATURE
CmsRetId  atcmd_qadc(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_adcdet(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_ricfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_gtvrictr(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_pircfg(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_time(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FFW_CHANGCHENG_AT_SUPPORT
CmsRetId  atcmd_mcuota(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_nfwupd(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FFW_WEBSOCKET_AT_ENABLE
CmsRetId  atcmd_GTWSRSET(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTWSHEAD(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTWSOPEN(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTWSSEND(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTWSCLOSE(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTWSREAD(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FIBOCOM_CHANGCHENG
CmsRetId  atcmd_mufota(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_CUST_KR_FEATURE
CmsRetId  atcmd_GTCDRX(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTSIMOTA(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTWDTRST(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_CUST_HELLOBIKE_FEATURE
CmsRetId  atcmd_LOCSUPPORT(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_LOCIPR(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_LOCVER(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_LOCSTATUS(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_INSOPEN(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_INSINFO(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_INSCLOSE(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GNTXT(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_LOCOPEN(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_LOCCLOSE(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_LOCATION(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GTGPS(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GNSSMODE(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GNSSVERSION(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GNSSTYPE(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GNSSFOTA(AtCmdInputContext *pAtCmdReq);
CmsRetId  atcmd_GNSSCMD(AtCmdInputContext *pAtCmdReq);
#endif

#ifdef CONFIG_FFW_FEATURE_LPA
CmsRetId  atcmd_GTEUICC(AtCmdInputContext *pAtCmdReq);
#endif
#endif /* BC6FC25D_D194_451E_A25F_B09A76C59CF4 */
