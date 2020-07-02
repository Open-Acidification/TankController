/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
 
#ifndef _ASYNCTASKLIB_h
#define _ASYNCTASKLIB_h
 
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

typedef void(*AsyncTaskCallback)();

class AsyncTask
{
 public:
	AsyncTask(unsigned long millisInterval);
	AsyncTask(unsigned long millisInterval, AsyncTaskCallback OnFinish);

	AsyncTask(unsigned long millisInterval, bool autoReset);
	AsyncTask(unsigned long millisInterval, bool autoReset, AsyncTaskCallback OnFinish);

	void Start();
	void Reset();
	void Stop();
	bool Update();
	void Update(AsyncTask &next);

	void SetIntervalMillis(unsigned long interval);
	void SetIntervalMicros(unsigned long interval);
	
	unsigned long GetStartTime();
	unsigned long GetElapsedTime();
	unsigned long GetRemainingTime();

	bool IsActive() const;
	bool IsExpired() const;
	
	unsigned long Interval;
	bool AutoReset;
	
	AsyncTaskCallback OnFinish;

private:
	bool _isActive;
	bool _isExpired;
	unsigned long _startTime;
};
#endif

