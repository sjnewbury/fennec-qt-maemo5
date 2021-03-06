/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MacLaunchHelper_h_
#define MacLaunchHelper_h_

extern "C" {
  void LaunchChildMac(int aArgc, char** aArgv, PRUint32 aRestartType = 0,
                      pid_t *pid = 0);
}

#endif
