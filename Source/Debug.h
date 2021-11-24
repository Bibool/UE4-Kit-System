#pragma once

/** Global log debug boolean. Setting to false will get rid of all Log messages. */
extern constexpr bool GWriteLogDebug = true;


#ifndef CAN_WRITE_LOG_DEBUG
/** A macro that begins and if statement using the value of gWriteLogDebug*/
	#define  CAN_WRITE_LOG_DEBUG if (GWriteLogDebug)
#endif 

#ifndef WRITE_DEBUG_LOG
/** A macro that wraps around UE_LOG with an added if statement. */
	#define  WRITE_DEBUG_LOG(p) if (GWriteLogDebug) { UE_LOG(LogTemp, Warning, (p) ); }
#endif 