import platform
from SCons.Script import *


def CheckOSXFramework(context, name):
    if (platform.system().lower() == 'darwin'):
        context.Message('Checking for framework %s... ' % name)
        save_FRAMEWORKS = context.env['FRAMEWORKS']
        context.env.PrependUnique(FRAMEWORKS = [name])
        result = \
            context.TryLink('int main(int argc, char **argv) {return 0;}', '.c')
        context.Result(result)

        if not result:
            context.env.Replace(FRAMEWORKS = save_FRAMEWORKS)

        return result

    context.Result(False)
    return False


def generate(env):
    env.CBAddTest('CheckOSXFramework', CheckOSXFramework)


def exists():
    return 1
