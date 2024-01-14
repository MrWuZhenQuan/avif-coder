/*
 * MIT License
 *
 * Copyright (c) 2023 Radzivon Bartoshyk
 * avif-coder [https://github.com/awxkee/avif-coder]
 *
 * Created by Radzivon Bartoshyk on 23/9/2023
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "VulkanRunner.h"
#include "JniException.h"

#include <dlfcn.h>
#include <mutex>

std::mutex vulkanMutex;

VulkanComputeRunnerFunc VulkanRunner;
VulkanComputeRunnerWithDataFunc VulkanRunnerWithData;

bool vulkanRunnerLoaded = false;

bool loadVulkanRunner() {
    if (androidOSVersion() < 29) {
        return false;
    }
    std::lock_guard guard(vulkanMutex);

    if (vulkanRunnerLoaded) {
        return VulkanRunner != nullptr;
    }

    vulkanRunnerLoaded = true;

    void *hhl = dlopen("libvulkanprocessor.so", RTLD_NOW);
    if (!hhl) {
        return false;
    }

    VulkanRunner = (VulkanComputeRunnerFunc) dlsym(hhl,
                                                   "ComputeKernel");
    if (VulkanRunner == nullptr) {
        return false;
    }

    VulkanRunnerWithData = (VulkanComputeRunnerWithDataFunc) dlsym(hhl,
                                                                   "ComputeKernelWithData");
    if (VulkanRunnerWithData == nullptr) {
        return false;
    }
    return true;
}