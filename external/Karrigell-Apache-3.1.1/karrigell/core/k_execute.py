
import sys
import email

import k_config
import k_environ
import k_version
from k_exceptions import *

_DEBUG = True

class FakeServer(object):
    def __init__(self, host):
        self.host = host
        self.port = 0
        
class FakeHTTP(object):
    def __init__ (self, host, config):
        self.config = config
        self.output_encoding = None
        self.managed_extensions = ".htm",".html",".py",".pih",".hip",".ks"
        self.method = ""
        #self.output = sys.stderr.write
        self.headers = email.message_from_string("")
        self.protocol = ""
        self.client_address = [host,0]
        self.server = FakeServer(host)
        self.ns = {"REQUEST_HANDLER":None,
            "SCRIPT_END":SCRIPT_END,
            "HTTP_REDIRECTION":HTTP_REDIRECTION,
            "Session":None,
            "Role":None,
            "PRINT":self._print,
            "STDOUT":self._std_out,
            "LOG":None,
            "ACCEPTED_LANGUAGES":None,
            "HEADERS":None,
            "RESPONSE":None,
            "COOKIE":None,
            "SET_COOKIE":None,
            "CONFIG":config
            }
    
    def version_string(self):
        return "Karrigell %s" %k_version.__version__
    
    def _print(self,*data):
        sys.stdout.write(" ".join(data)+"\n")

    def _std_out(self,*data):
        sys.stdout.write(" ".join(data))


def ExecuteManagementScript(host, url):
    import k_target
    
    config = k_config.get_host_conf(host)

    h = FakeHTTP(host, config)
    target = k_target.Target(h, url)
    
    # environment variables (same as os.environ for CGI scripts)
    env = k_environ.make_environ(h, target)

    h.ns["ENVIRON"] = env
    
    # CGI scripts
    if target.is_cgi():
        os.environ.update(env) # set environment variables

        try:
            execfile(target.name)
        except:
            pass
        return
    
    try:
        # execute Python script in namespace
        target.parse_script()
        # run script
        target.run(h.ns)
    except :
        if _DEBUG :
            raise
    

def ExecuteStartupModules ():
    for host,modules in k_config.config.iteritems():
        for m in modules.startup_modules :
            print host, "-->", m
            ExecuteManagementScript(host, m)
        
def ExecuteShutdownModules ():
    for host,modules in k_config.config.iteritems():
        for m in modules.shutdown_modules :
            print host, "-->", m
            ExecuteManagementScript(host, m)
        
