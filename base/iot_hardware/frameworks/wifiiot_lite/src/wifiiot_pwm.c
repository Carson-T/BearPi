/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "wifiiot_errno.h"
#include "hal_wifiiot_pwm.h"
#include "wifiiot_pwm.h"

unsigned int PwmInit(WifiIotPwmPort port)
{
    return HalPwmInit((HalWifiIotPwmPort)port);
}

unsigned int PwmDeinit(WifiIotPwmPort port)
{
    return HalPwmDeinit((HalWifiIotPwmPort)port);
}

unsigned int PwmSetClock(WifiIotPwmClkSource clkSource)
{
    return HalPwmSetClock((HalWifiIotPwmClkSource)clkSource);
}

unsigned int PwmStart(WifiIotPwmPort port, unsigned short duty, unsigned short freq)
{
    return HalPwmStart((HalWifiIotPwmPort)port, duty, freq);
}

unsigned int PwmStop(WifiIotPwmPort port)
{
    return HalPwmStop((HalWifiIotPwmPort)port);
}

