/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "AsyncTaskLib.h"

AsyncTask::AsyncTask(unsigned long millisInterval) : AsyncTask(millisInterval, false, nullptr)
{
}

AsyncTask::AsyncTask(unsigned long millisInterval, AsyncTaskCallback onFinish) : AsyncTask(millisInterval, false, onFinish)
{

}

AsyncTask::AsyncTask(unsigned long millisInterval, bool autoReset) : AsyncTask(millisInterval, false, nullptr)
{
}

AsyncTask::AsyncTask(unsigned long millisInterval, bool autoReset, AsyncTaskCallback onFinish)
{
	Interval = 1000 * millisInterval;
	AutoReset = autoReset;
	OnFinish = onFinish;
}

void AsyncTask::Start()
{
	Reset();
	_isActive = true;
}

void AsyncTask::Reset()
{
	_startTime = micros();
}

void AsyncTask::Stop()
{
	_isActive = false;
}

bool AsyncTask::Update()
{
	if (_isActive == false) return false;

	if (static_cast<unsigned long>(micros() - _startTime) >= Interval)
	{
		if (OnFinish != nullptr) OnFinish();

		_isExpired = !AutoReset;
		_isActive = AutoReset;
		Reset();
	}
	return _isExpired;
}

void AsyncTask::Update(AsyncTask &next)
{
	if (Update())
	{
		_isExpired = false;
		next.Start();
	}
}

void AsyncTask::SetIntervalMillis(unsigned long interval)
{
	Interval = 1000 * interval;
}

void AsyncTask::SetIntervalMicros(unsigned long interval)
{
	Interval = interval;
}

unsigned long AsyncTask::GetStartTime()
{
	return _startTime;
}

unsigned long AsyncTask::GetElapsedTime()
{
	return micros() - _startTime;
}

unsigned long AsyncTask::GetRemainingTime()
{
	return Interval - micros() + _startTime;
}

bool AsyncTask::IsActive() const
{
	return _isActive;
}

bool AsyncTask::IsExpired() const
{
	return _isExpired;
}
