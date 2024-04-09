void raiseErr(const char * func, const char *errMsg);
void raiseErrAndSource(const char * func, const char *errMsg, const char *errSource);
void raiseErrno(const char * func, const char *errMsg, const char *errnoPath);
void raiseErrnoBypassPath(const char * func, const char *errMsg);