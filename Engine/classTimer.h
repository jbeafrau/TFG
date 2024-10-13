#ifndef CLASSTIMER_H
#define CLASSTIMER_H

#include <SDL.h>
#include <stdio.h>
#include <string>
#include <sstream>

class classTimer
{
   public:
		//Initializes variables
	   classTimer();

		//The various clock actions
		void start();
		void stop();
		void reset();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};

#endif // CLASSTIMER_H
