#!/usr/bin/env python3
import os
import subprocess
exe_mt = "stress_test_multi"
exe_st = "stress_test_single"
fn = os.path.join(os.path.dirname(__file__), "../cmake-build-debug/project/testing/stress_test/")
subprocess.call(fn + exe_mt)
