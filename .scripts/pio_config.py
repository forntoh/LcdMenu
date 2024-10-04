import os

from SCons.Script import Import

Import("env")

if os.getenv("LOCAL_BUILD"):
    env.Append(CPPDEFINES=["DEBUG"])