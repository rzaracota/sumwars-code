#ifndef GUS_LIB_TIMER_H
#define GUS_LIB_TIMER_H

//   This file is part of the guslib library, licensed under the terms of the MIT License.
//
//   The MIT License
//   Copyright (C) 2010-2011  Augustin Preda (thegusty999@gmail.com)
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy
//   of this software and associated documentation files (the "Software"), to deal
//   in the Software without restriction, including without limitation the rights
//   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//   copies of the Software, and to permit persons to whom the Software is
//   furnished to do so, subject to the following conditions:
//   
//   The above copyright notice and this permission notice shall be included in
//   all copies or substantial portions of the Software.
//   
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//   THE SOFTWARE.
//
//   Timer utility.
//
//	 Last change:	$LastChangedDate: 2012-09-16 11:26:05 +0300 (D, 16 sep. 2012) $
//	 Revision:		$Revision: 541 $

#include <gussoundutil/guslibbuildopts.h>

#include <gussoundutil/singleton.hpp>

#include <ostream>

#if GUSLIB_FLAG_MULTITHREAD
#include <gussoundutil/thread.h>
#endif

namespace guslib
{
	typedef long long TimeUnits;


	/// Abstract timer class, has the entire timer functionality and logic, but has no access
	/// to time passing functions.
	class AbstractTimer
	{
	protected:
		TimeUnits  latestTime;       // latest accessed time

	public:
		/// Constructor.
		AbstractTimer();

		/// Destructor.
		virtual ~AbstractTimer();

		/// Access the system time units. Override in child classes.
		virtual TimeUnits getCurrentTimeUnits() const = 0;

		/// Resets the timer duration to zero
		virtual void reset();

		/// Gets the time interval from the last reset operation
		virtual TimeUnits elapsed();

		/// Gets the time interval from the last reset operation and then resets the timer
		virtual TimeUnits renew();
	};


#ifdef _WINDOWS
	/// Windows implementation for the abstract timer.
	class WinTimer : public AbstractTimer
	{
	protected:
	public:
		WinTimer();
		virtual ~WinTimer();
		virtual TimeUnits getCurrentTimeUnits () const;
		virtual TimeUnits getTimeSinceMidnight () const;
	};
#endif

#ifdef _WINDOWS
	// The windows timer will be the standard accessible timer, through the Timer name.
	typedef WinTimer Timer;
#else
	// TODO: define a unix timer?
#endif

	class ApplicationClockUtil;


	///
	/// The application clock utility class. 
	///
	class ApplicationClockUtil
		: public Timer
	{
	protected:
		ApplicationClockUtil ();
		TimeUnits  initialTime_;

	public:
		virtual ~ApplicationClockUtil ();
		friend class guslib::Singleton <ApplicationClockUtil>;

		virtual TimeUnits getTimeFromStart () const;

		/**
			Make the class accessible to ostreams (e.g. cout);
			Because this can be used very often in the code, it needs to be very fast.
		*/
		inline friend std::ostream & operator<<(std::ostream& s, const ApplicationClockUtil & cl)
		{
			TimeUnits temp (cl.getTimeFromStart ());

			int nMillis = temp % 1000;
			temp = (temp - nMillis) / 1000;

			int nSec = temp % 60;
			temp = (temp - nSec) / 60;

			int nMin = temp % 60;
			temp = (temp - nMin) / 60;

			int nHour = temp % 24;
			temp = (temp - nHour) / 24;

			char caster [4];
			caster[3] = '\0';

			caster[0] = nHour / 10 + '0';
			caster[1] = nHour % 10 + '0';
			caster[2] = '\0';
			s << caster << ":";

			caster[0] = nMin / 10 + '0';
			caster[1] = nMin % 10 + '0';
			caster[2] = '\0';
			s << caster << ":";

			caster[0] = nSec / 10 + '0';
			caster[1] = nSec % 10 + '0';
			caster[2] = '\0';
			s << caster << ".";

			caster[0] = nMillis / 100;
			caster[2] = nMillis % 10 + '0';
			caster[1] = (nMillis - caster[0] * 100) / 10 + '0';
			caster[0] += '0';
			caster[3] = '\0';

			s << caster;
			return s;
		}

		virtual std::string getTimeAsString () const;
	};


	typedef guslib::Singleton <ApplicationClockUtil> AppClock;


	///
	/// The system clock utility class. 
	///
	class SystemClockUtil
		: public ApplicationClockUtil
	{
	protected:
		SystemClockUtil ();

	public:
		friend class guslib::Singleton <SystemClockUtil>;

		// gets the time since the start of day.
		virtual TimeUnits getTimeFromStart () const;

		/**
			Make the class accessible to ostreams (e.g. cout);
			Because this can be used very often in the code, it needs to be very fast.
		*/
		inline friend std::ostream & operator<<(std::ostream& s, const SystemClockUtil & cl)
		{
			TimeUnits temp (cl.getTimeFromStart ());

			int nMillis = temp % 1000;
			temp = (temp - nMillis) / 1000;

			int nSec = temp % 60;
			temp = (temp - nSec) / 60;

			int nMin = temp % 60;
			temp = (temp - nMin) / 60;

			int nHour = temp % 24;
			temp = (temp - nHour) / 24;

			char caster [4];
			caster[3] = '\0';

			caster[0] = nHour / 10 + '0';
			caster[1] = nHour % 10 + '0';
			caster[2] = '\0';
			s << caster << ":";

			caster[0] = nMin / 10 + '0';
			caster[1] = nMin % 10 + '0';
			caster[2] = '\0';
			s << caster << ":";

			caster[0] = nSec / 10 + '0';
			caster[1] = nSec % 10 + '0';
			caster[2] = '\0';
			s << caster << ".";

			caster[0] = nMillis / 100;
			caster[2] = nMillis % 10 + '0';
			caster[1] = (nMillis - caster[0] * 100) / 10 + '0';
			caster[0] += '0';
			caster[3] = '\0';

			s << caster;
			return s;
		}

		/**
			Make the class accessible to ostreams (e.g. cout);
			Because this can be used very often in the code, it needs to be very fast.
		*/
		inline friend std::wostream & operator<<(std::wostream& s, const SystemClockUtil & cl)
		{
			TimeUnits temp (cl.getTimeFromStart ());

			int nMillis = temp % 1000;
			temp = (temp - nMillis) / 1000;

			int nSec = temp % 60;
			temp = (temp - nSec) / 60;

			int nMin = temp % 60;
			temp = (temp - nMin) / 60;

			int nHour = temp % 24;
			temp = (temp - nHour) / 24;

			char caster [4];
			caster[3] = '\0';

			caster[0] = nHour / 10 + '0';
			caster[1] = nHour % 10 + '0';
			caster[2] = '\0';
			s << caster << ":";

			caster[0] = nMin / 10 + '0';
			caster[1] = nMin % 10 + '0';
			caster[2] = '\0';
			s << caster << ":";

			caster[0] = nSec / 10 + '0';
			caster[1] = nSec % 10 + '0';
			caster[2] = '\0';
			s << caster << ".";

			caster[0] = nMillis / 100;
			caster[2] = nMillis % 10 + '0';
			caster[1] = (nMillis - caster[0] * 100) / 10 + '0';
			caster[0] += '0';
			caster[3] = '\0';

			s << caster;
			return s;
		}
	};


	typedef guslib::Singleton <SystemClockUtil> SysClock;

}


#endif // GUS_LIB_TIMER_H