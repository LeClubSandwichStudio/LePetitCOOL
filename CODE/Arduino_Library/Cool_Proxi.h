/*	LACOOL_CO's GENERAL LIBRARY
	TYPE : SENSOR	-	MODEL : Grove Ultrasonic Ranger
	ROLE : Le Petit Cool Water Level Sensor
	@author Maximilien Daman	15/11/2015
	Mail : max@lacool.co	Web : www.lacool.co
 */

 #ifndef 	Cool_Proxi_h
#define 	Cool_Proxi_h

#include "Arduino.h"

	class Cool_Proxi
	{
		public:
			Cool_Proxi();
			void 		begin(uint8_t Pin);
			uint8_t		errorlevel();
			void 		ReadVal_cm();
			inline long	getVal_cm()	{	return Level;	}
		private:
			uint8_t 	WLPin;
			uint8_t 	ErrorLevel;
			long		Level;
			long		PulseDuration;
	};
#endif
