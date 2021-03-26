from __future__ import print_function

import os
import sys
import traceback
from SCons.Script import *
import inspect
import types
import re


def CBCheckEnv(ctx, name, require = False):
    ctx.did_show_result = 1

    if name in os.environ: return os.environ[name]
    elif require: raise Exception("Missing environment variable: " + name)


def CBRequireEnv(ctx, name):
    ctx.did_show_result = 1
    return ctx.sconf.CBCheckEnv(name, True)


def CBCheckEnvPath(ctx, name):
    ctx.did_show_result = 1
    paths = ctx.sconf.CBCheckEnv(name)

    existing = []
    if paths:
        for path in paths.split(';'):
            if os.path.isdir(path): existing.append(path)

    return existing


def CBCheckPathWithSuffix(ctx, base, suffixes):
    ctx.did_show_result = 1
    if suffixes is None: return []
    if not type(suffixes) in (list, tuple): suffixes = [suffixes]
    existing = []
    for suffix in suffixes:
        if os.path.isdir(base + suffix): existing.append(base + suffix)
    return existing


def CBCheckHome(ctx, name, inc_suffix = '/include', lib_suffix = '/lib',
                require = False, suffix = '_HOME'):
    ctx.did_show_result = 1
    name = name.upper().replace('-', '_')

    # Include
    user_inc = ctx.sconf.CBCheckEnvPath(name + '_INCLUDE')
    for path in user_inc:
        ctx.env.AppendUnique(CPPPATH = [path])
        require = False

    # Lib path
    user_libpath = ctx.sconf.CBCheckEnvPath(name + '_LIBPATH')
    for path in user_libpath:
        ctx.env.Prepend(LIBPATH = [path])
        require = False

    # Link flags
    linkflags = ctx.sconf.CBCheckEnv(name + '_LINKFLAGS')
    if linkflags: ctx.env.AppendUnique(LINKFLAGS = linkflags.split())

    # Home
    home = ctx.sconf.CBCheckEnv(name + suffix, require)
    if home:
        if not user_inc:
            for path in ctx.sconf.CBCheckPathWithSuffix(home, inc_suffix):
                ctx.env.AppendUnique(CPPPATH = [path])

        if not user_libpath:
            for path in ctx.sconf.CBCheckPathWithSuffix(home, lib_suffix):
                ctx.env.Prepend(LIBPATH = [path])

    return home


def CBRequireHome(ctx, name, inc_suffix = '/include', lib_suffix = '/lib',
                  suffix = '_HOME'):
    ctx.did_show_result = 1
    return ctx.sconf.CBCheckHome(name, inc_suffix, lib_suffix, True, suffix)


def CBCheckLib(ctx, lib, unique = False, append = False, **kwargs):
    ctx.did_show_result = 1

    # Lib path
    libpath = ctx.sconf.CBCheckEnv(lib.upper() + '_LIBPATH')
    if libpath: ctx.env.Prepend(LIBPATH = [libpath])

    # Lib name
    libname = ctx.sconf.CBCheckEnv(lib.upper() + '_LIBNAME')
    if libname: lib = libname

    if ctx.sconf.CheckLib(lib, autoadd = 0, **kwargs):
        if unique: ctx.env.PrependUnique(LIBS = [lib])
        else: ctx.env.Prepend(LIBS = [lib])

        return True

    return False


def CBRequireLib(ctx, lib, **kwargs):
    ctx.did_show_result = 1
    if not ctx.sconf.CBCheckLib(lib, **kwargs):
        raise Exception('Need library ' + lib)


def CBCheckHeader(ctx, hdr, **kwargs):
    ctx.did_show_result = 1
    return ctx.sconf.CheckHeader(hdr, **kwargs)


def CBRequireHeader(ctx, hdr, **kwargs):
    ctx.did_show_result = 1
    if not ctx.sconf.CheckHeader(hdr, **kwargs):
        raise Exception('Need header ' + hdr)


def CBCheckCHeader(ctx, hdr, **kwargs):
    ctx.did_show_result = 1
    return ctx.sconf.CheckCHeader(hdr, **kwargs)


def CBRequireCHeader(ctx, hdr, **kwargs):
    ctx.did_show_result = 1
    if not ctx.sconf.CheckCHeader(hdr, **kwargs):
        raise Exception('Need C header ' + hdr)


def CBCheckCXXHeader(ctx, hdr, **kwargs):
    ctx.did_show_result = 1
    return ctx.sconf.CheckCXXHeader(hdr, **kwargs)


def CBRequireCXXHeader(ctx, hdr, **kwargs):
    ctx.did_show_result = 1
    if not ctx.sconf.CheckCXXHeader(hdr, **kwargs):
        raise Exception('Need C++ header ' + hdr)


def CBCheckFunc(ctx, func, **kwargs):
    ctx.did_show_result = 1
    return ctx.sconf.CheckFunc(func, **kwargs)


def CBRequireFunc(ctx, func, **kwargs):
    ctx.did_show_result = 1
    if not ctx.sconf.CheckFunc(func, **kwargs):
        raise Exception('Need Function ' + func)


def CBConfig(ctx, name, required = True, **kwargs):
    ctx.did_show_result = 1
    env = ctx.env
    conf = ctx.sconf

    if name in env.cb_methods:
        ret = False
        try:
            conf.env = env.Clone()
            ret = conf.env.cb_methods[name](conf, **kwargs)
            if ret is None: ret = True

            # Commit changes
            if ret: env.Replace(**conf.env.Dictionary())

        except Exception as e:
            if required: raise
            ctx.Message(str(e))

        finally:
            conf.env = env # Put back master env

        if ret: env.cb_enabled.add(name)
        elif required: raise Exception('Failed to configure ' + name)

        return ret

    elif required:
        raise Exception('Config method not defined for tool ' + name)

    return False


def CBTryLoadTool(env, name, path):
    if name in env.cb_loaded: return True

    if not os.path.exists(str(path) + '/' + name + '/__init__.py'):
        return False

    try:
        env.cb_paths.append(path)
        env.cb_loaded.add(name)
        env.Tool(name, toolpath = [path])
        env.cb_paths.pop()
        return True

    except Exception as e:
        traceback.print_exc()
        env.cb_loaded.remove(name)
        env.cb_paths.pop()
        return False


def CBLoadTool(env, name, paths = []):
    if name in env.cb_loaded: return True

    if hasattr(paths, 'split'): paths = paths.split()
    else: paths = list(paths)
    paths += env.cb_paths

    home_env_var = name.upper().replace('-', '_') + '_HOME'

    home = os.environ.get(home_env_var, None)
    if home is not None: paths.insert(0, home + '/config')

    path = inspect.getfile(inspect.currentframe())
    cd = os.path.dirname(os.path.abspath(path))
    paths.append(cd)

    paths.append('./config')

    for path in paths:
        if env.CBTryLoadTool(name, path): return True

    msg = 'Failed to load tool ' + name + ' from the following paths:'
    for path in paths: msg += '\n  ' + path
    if home is None:
        msg += '\nHave you set ' + home_env_var + '?'

    print(msg)
    raise Exception('Failed to load tool ' + name)


def CBLoadTools(env, tools, paths = []):
    if hasattr(tools, 'split'): tools = tools.split()
    for name in tools: env.CBLoadTool(name, paths)


def CBDefine(env, defs):
    if not type(defs) in (list, tuple): defs = [defs]
    env.AppendUnique(CPPDEFINES = defs)


def CBConfigDef(env, defs):
    if not type(defs) in (list, tuple): defs = [defs]
    env.cb_config_defs.update(defs)


def CBWriteConfigDef(env, path):
    if not os.path.exists(os.path.dirname(path)):
        os.makedirs(os.path.dirname(path))

    with open(path, 'w') as f:
        f.write('// Autogenerated file\n')
        f.write('#pragma once\n\n')

        for var in env.cb_config_defs:
            f.write('#define %s\n' % var)


def CBAddVariables(env, *args):
    env.cb_vars += args


def CBAddTest(env, name, func = None):
    if func is None:
        func = name
        name = func.__name__

    env.cb_tests[name] = func


def CBAddConfigTest(env, name, func):
    env.cb_methods[name] = func


def CBConfigEnabled(env, name):
    return name in env.cb_enabled


def on_config_finish(conf):
    env = conf.env
    for cb in env.cb_finish_cbs: cb(env)
    conf.OrigFinish()
    if env.get('gen_ninja', False): env.GenerateNinja()


updated_csig = set()


def decider_hack(dep, target, prev_ni, *args, **kwargs):
    from SCons.Util import MD5signature

    #print('%s ?= %s %s' % (dep, target, prev_ni.csig))

    ninfo = dep.get_ninfo();

    # Make sure csigs get updated
    if not hasattr(ninfo, 'csig') or str(dep) not in updated_csig:
        ninfo.csig = MD5signature(dep.get_contents())
        updated_csig.add(str(dep))

    # .csig may not exist, because no target was built yet...
    if not hasattr(prev_ni, 'csig'): return True

    # Target file may not exist yet
    if not os.path.exists(str(target.abspath)): return True

    # Some change on source file => update installed one
    if ninfo.csig != prev_ni.csig: return True

    return False


def CBConfigure(env):
    env.Decider(decider_hack)

    env.CBLoadTool('test')
    env.CBLoadTool('ninja')

    conf = Configure(env)

    for name, test in env.cb_tests.items():
        conf.AddTest(name, test)

    # Load config files
    configs = []

    if 'SCONS_OPTIONS' in os.environ:
        options = os.environ['SCONS_OPTIONS']
        if not os.path.exists(options):
            print('options file "%s" set in SCONS_OPTIONS does not exist' %
                  options)
            Exit(1)

        configs.append(options)

    if os.path.exists('default-options.py'):
        configs.append('default-options.py')
    if os.path.exists('options.py'): configs.append('options.py')

    # Load variables
    v = Variables(configs)
    v.AddVariables(*env.cb_vars)
    v.Update(env)

    Help(v.GenerateHelpText(env))

    # Override Finish
    conf.OrigFinish = conf.Finish
    conf.Finish = types.MethodType(on_config_finish, conf)

    for cb in env.cb_configure_cbs: cb(env)

    return conf


def CBDownload(env, target, url):
    try:
        import urllib # Python 3+
    except ImportError:
        import urllib2 as urllib

    sys.stdout.write('Downloading ' + url + '.')
    sys.stdout.flush()

    ftp_proxy = os.getenv('ftp_proxy', None)
    http_proxy = os.getenv('http_proxy', None)

    if ftp_proxy or http_proxy:
        handlers = {}
        if ftp_proxy: handlers['ftp'] = ftp_proxy
        if http_proxy: handlers['http'] = http_proxy

        opener = urllib.build_opener(urllib.ProxyHandler(handlers))
        urllib.install_opener(opener)

    f = None
    stream = None
    try:
        stream = urllib.urlopen(url)
        f = open(target, 'wb', 0) # Unbuffered
        while stream and f:
            data = stream.read(1024 * 1024)
            if not data: break
            f.write(data)
            sys.stdout.write('.')
            sys.stdout.flush()

        sys.stdout.write('ok\n')
        sys.stdout.flush()

    finally:
        if f is not None: f.close()
        if stream is not None: stream.close()


def CBAddConfigFinishCB(env, cb):
    env.cb_finish_cbs.append(cb)


def CBAddConfigureCB(env, cb):
    env.cb_configure_cbs.append(cb)


def CBBuildSetRegex(env, pats):
    if hasattr(pats, 'split'): pats = pats.split()
    return re.compile('^(' + ')|('.join(pats) + ')$')


def generate(env):
    major, minor = 3, 1
    v = sys.version_info
    if v.major < major or (v.major == major and v.minor < minor):
        raise Exception("C! requires Python %d.%d+ found %d.%d." % (
            major, minor, v.major, v.minor))

    # Add member variables
    env.cb_loaded = set()
    env.cb_enabled = set()
    env.cb_methods = {}
    env.cb_deps_methods = {}
    env.cb_tests = {}
    env.cb_vars = []
    env.cb_paths = []
    env.cb_finish_cbs = []
    env.cb_configure_cbs = []
    env.cb_config_defs = set()

    # Add methods
    env.AddMethod(CBTryLoadTool)
    env.AddMethod(CBLoadTool)
    env.AddMethod(CBLoadTools)
    env.AddMethod(CBDefine)
    env.AddMethod(CBConfigDef)
    env.AddMethod(CBWriteConfigDef)
    env.AddMethod(CBAddVariables)
    env.AddMethod(CBAddTest)
    env.AddMethod(CBAddConfigTest)
    env.AddMethod(CBConfigEnabled)
    env.AddMethod(CBConfigure)
    env.AddMethod(CBDownload)
    env.AddMethod(CBAddConfigFinishCB)
    env.AddMethod(CBAddConfigureCB)
    env.AddMethod(CBBuildSetRegex)

    # Add tests
    env.CBAddTest(CBCheckEnv)
    env.CBAddTest(CBRequireEnv)
    env.CBAddTest(CBCheckEnvPath)
    env.CBAddTest(CBCheckPathWithSuffix)
    env.CBAddTest(CBCheckHome)
    env.CBAddTest(CBRequireHome)
    env.CBAddTest(CBCheckLib)
    env.CBAddTest(CBRequireLib)
    env.CBAddTest(CBCheckHeader)
    env.CBAddTest(CBRequireHeader)
    env.CBAddTest(CBCheckCHeader)
    env.CBAddTest(CBRequireCHeader)
    env.CBAddTest(CBCheckCXXHeader)
    env.CBAddTest(CBRequireCXXHeader)
    env.CBAddTest(CBCheckFunc)
    env.CBAddTest(CBRequireFunc)
    env.CBAddTest(CBConfig)


def exists(env):
    return 1
