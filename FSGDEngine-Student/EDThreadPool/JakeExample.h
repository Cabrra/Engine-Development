#pragma once

#include "../EDUtilities/InternalOutput.h"
#include "WorkerThread.h"

namespace EDThreadPool
{
	class JakeExample :
		public WorkerThread
	{
public:
	int radix;
	int prime_sum;

public:
	JakeExample(int _radix)
		{
			radix = _radix;
			prime_sum = 3;
		}

		void DoneCallback()
		{
			EDUtilities::InternalOutput::GetReference() << "Thread Pool found " << prime_sum << " as the total sum of primes between 1 and " << radix << "\n";
		}

		void DoWork()
		{
			for (int i = 2; i < radix; i++)
			{
				bool is_prime = true;
				int num = 0;
				for (int j = 2; j < (int)(radix-1); j++)
				{
					// if the divisor has grown to size of int we are checking
					num = i;
					if (j >= i)
					{
						break;
					}

					// if it is divisible must not be prime
					if (i % j == 0)
					{
						is_prime = false;
						break;
					}
					
				}

				if (is_prime == true)
				{
					prime_sum += num;
				}
			}
		}
	};
}
