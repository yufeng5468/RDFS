/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * Dump utilities for erasure coders.
 */

#ifndef ISAL_INCLUDE_DUMP_H_
#define ISAL_INCLUDE_DUMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "erasure_coder.h"

void dumpEncoder(IsalEncoder *pCoder);

void dumpDecoder(IsalDecoder *pCoder);

void dump(unsigned char *buf, int len);

void dumpMatrix(unsigned char **s, int k, int m);

void dumpCodingMatrix(unsigned char *s, int n1, int n2);

#endif  // ISAL_INCLUDE_DUMP_H_
