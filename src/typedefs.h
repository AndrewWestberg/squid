
typedef unsigned int store_status_t;
typedef unsigned int mem_status_t;
typedef unsigned int ping_status_t;
typedef unsigned int swap_status_t;

typedef struct {
    size_t bytes;
    size_t kb;
} kb_t;

/*
 * grep '^struct' structs.h \
 * | perl -ne '($a,$b)=split;$c=$b;$c=~s/^_//; print "typedef struct $b $c;\n";'
 */

typedef struct _acl_ip_data acl_ip_data;
typedef struct _acl_time_data acl_time_data;
typedef struct _acl_name_list acl_name_list;
typedef struct _acl_deny_info_list acl_deny_info_list;
typedef struct _acl_proxy_auth acl_proxy_auth;
typedef struct _acl acl;
typedef struct _snmpconf snmpconf;
typedef struct _acl_list acl_list;
typedef struct _acl_access acl_access;
typedef struct _aclCheck_t aclCheck_t;
typedef struct _aio_result_t aio_result_t;
typedef struct _wordlist wordlist;
typedef struct _intlist intlist;
typedef struct _ushortlist ushortlist;
typedef struct _relist relist;
typedef struct _SquidConfig SquidConfig;
typedef struct _SquidConfig2 SquidConfig2;
typedef struct _close_handler close_handler;
typedef struct _dread_ctrl dread_ctrl;
typedef struct _dnsserver_t dnsserver_t;
typedef struct _dnsStatData dnsStatData;
typedef struct _dwrite_q dwrite_q;
typedef struct _fde fde;
typedef struct _fileMap fileMap;
typedef struct _fqdncache_entry fqdncache_entry;
typedef struct _hash_link hash_link;
typedef struct _hash_table hash_table;
#if 0 /* use new interfaces */
typedef struct _http_reply http_reply;
#else
typedef struct _HttpReply http_reply;
#endif
typedef struct _HttpStateData HttpStateData;
typedef struct _icpUdpData icpUdpData;
typedef struct _clientHttpRequest clientHttpRequest;
typedef struct _ConnStateData ConnStateData;
typedef struct _ipcache_addrs ipcache_addrs;
typedef struct _ipcache_entry ipcache_entry;
typedef struct _domain_ping domain_ping;
typedef struct _domain_type domain_type;
typedef struct _peer peer;
typedef struct _net_db_name net_db_name;
typedef struct _net_db_peer net_db_peer;
typedef struct _netdbEntry netdbEntry;
typedef struct _icp_ping_data icp_ping_data;
typedef struct _ps_state ps_state;
typedef struct _HierarchyLogEntry HierarchyLogEntry;
typedef struct _pingerEchoData pingerEchoData;
typedef struct _pingerReplyData pingerReplyData;
typedef struct _icp_common_t icp_common_t;
typedef struct _Meta_data Meta_data;
typedef struct _iostats iostats;
typedef struct _mem_node mem_node;
typedef struct _mem_hdr mem_hdr;
typedef struct _store_client store_client;
typedef struct _MemObject MemObject;
typedef struct _StoreEntry StoreEntry;
typedef struct _SwapDir SwapDir;
typedef struct _request_t request_t;
typedef struct _AccessLogEntry AccessLogEntry;
typedef struct _cachemgr_passwd cachemgr_passwd;
typedef struct _refresh_t refresh_t;
typedef struct _CommWriteStateData CommWriteStateData;
typedef struct _ErrorState ErrorState;
typedef struct _dlink_node dlink_node;
typedef struct _dlink_list dlink_list;
typedef struct _StatCounters StatCounters;
typedef struct _tlv tlv;
typedef struct _storeSwapLogData storeSwapLogData;
typedef struct _cacheSwap cacheSwap;
typedef struct _StatHist StatHist;

/* define AIOCB even without USE_ASYNC_IO */
typedef void AIOCB(void *, int aio_return, int aio_errno);
typedef void CWCB(int fd, char *, size_t size, int flag, void *data);
typedef void CNCB(int fd, int status, void *);
typedef void FREE(void *);
typedef void FOCB(void *, int fd, int errcode);
typedef void EVH(void *);
typedef void PF(int, void *);
typedef void DRCB(int fd, const char *buf, int size, int errflag, void *data);
typedef void DWCB(int, int, size_t, void *);
typedef void FQDNH(const char *, void *);
typedef int HASHCMP(const void *, const void *);
typedef unsigned int HASHHASH(const void *, unsigned int);
typedef void IDCB(void *);
typedef void IPH(const ipcache_addrs *, void *);
typedef void IRCB(peer *, peer_t, icp_common_t *, void *data);
typedef void PSC(peer *, void *);
typedef void RH(void *data, char *);
typedef void UH(void *data, wordlist *);
typedef int DEFER(int fd, void *data);

typedef void SIH(int fd, void *);	/* swap in */
typedef int QS(const void *, const void *);	/* qsort */
typedef void STCB(void *, char *, ssize_t);	/* store callback */
typedef void STABH(void *);
typedef void ERCB(int fd, void *, size_t);
typedef void OBJH(StoreEntry *);
typedef void SIGHDLR(int sig);
typedef void STVLDCB(void *, int, int);

typedef double (*hbase_f)(double);
typedef void (*StatHistBinDumper)(StoreEntry *, int idx, double val, double size, int count);
/* MD5 cache keys */
typedef unsigned char cache_key;

/* context-based debugging, the actual type is subject to change */
typedef int Ctx;
