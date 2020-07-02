/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "AsyncTaskLib.h"

AsyncTask asyncTask1(1000, []() { digitalWrite(LED_BUILTIN, HIGH); });
AsyncTask asyncTask2(2000, []() { digitalWrite(LED_BUILTIN, LOW); });

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);

	asyncTask1.Start();
}

void loop()
{
	asyncTask1.Update(asyncTask2);
	asyncTask2.Update(asyncTask1);
}
