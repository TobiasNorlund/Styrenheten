#define OK_SENSOR_VALUE(x) (((x) != 0)||((x) != 255))

void calcAngle(int8_t dir)
{
	uint8_t sensorLongForward=getSensorLongForward();
	uint8_t sensorLongRear=getSensorLongRear();
	uint8_t sensorLongLeft=getSensorLongLeft();
	uint8_t sensorLongRight=getSensorLongRight();
	uint8_t sensorShortLeftForward=getSensorShortLeftForward();
	uint8_t sensorShortLeftRear=getSensorShortLeftRear();
	uint8_t sensorShortRightForward=getSensorShortRightForward();
	uint8_t sensorShortRightRear=getSensorShortRightRear();

	//front
	uint8_t frontAngleK=0;
	int8_t frontAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortLeftForward)&&OK_SENSOR_VALUE(sensorShortRightForward))
	{
		frontAngleK=calcKSennsorShort(sensorShortLeftForward)*calcKSennsorShort(sensorShortRightForward);
		frontAngleDiff=currentAngle-calcOpositeSensors(sensorShortRightForward,sensorShortLeftForward,dir);
	}

	//middle
	uint8_t middleAngleK=0;
	int8_t middleAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorLongLeft)&&OK_SENSOR_VALUE(sensorLongRight))
	{
		middleAngleK=calcKSennsorLong(sensorLongLeft)*calcKSennsorLong(sensorLongRight);
		middleAngleDiff=currentAngle-calcOpositeSensors(sensorLongRight,sensorLongLeft,dir);
	}
	//back
	uint8_t backAngleK=0;
	int8_t backAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortLeftRear)&&OK_SENSOR_VALUE(sensorShortRightRear))
	{
		backAngleK=calcKSennsorShort(sensorShortLeftRear)*calcKSennsorShort(sensorShortRightRear);
		backAngleDiff=currentAngle-calcOpositeSensors(sensorShortRightRear,sensorShortLeftRear,dir);
	}
	//right 2 short
	uint8_t right36AngleK=0;
	int8_t right36AngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortRightForward)&&OK_SENSOR_VALUE(sensorShortRightRear))
	{
		right36AngleK=calcKSennsorShort(sensorShortRightForward)*calcKSennsorShort(sensorShortRightRear);
		right36AngleDiff=currentAngle-calcSideSensors36(sensorShortRightForward,sensorShortRightRear,dir);
	}

	//right front short long 
	uint8_t right19FAngleK=0;
	int8_t right19FAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortRightForward)&&OK_SENSOR_VALUE(sensorLongRight))
	{
		right19FAngleK=calcKSennsorShort(sensorShortRightForward)*calcKSennsorLong(sensorLongRight);
		right19FAngleDiff=currentAngle-calcSideSensors19(sensorShortRightForward,sensorLongRight,dir);
	}

	//right front short long 
	uint8_t right19RAngleK=0;
	int8_t right19RAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortRightRear)&&OK_SENSOR_VALUE(sensorLongRight))
	{
		right19RAngleK=calcKSennsorShort(sensorShortRightRear)*calcKSennsorLong(sensorLongRight);
		right19RAngleDiff=currentAngle-calcSideSensors19(sensorLongRight,sensorShortRightRear,dir);
	}

	//left 2 short
	uint8_t left36AngleK=0;
	int8_t left36AngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortleftForward)&&OK_SENSOR_VALUE(sensorShortleftRear))
	{
		left36AngleK=calcKSennsorShort(sensorShortleftForward)*calcKSennsorShort(sensorShortleftRear);
		left36AngleDiff=currentAngle-calcSideSensors36(sensorShortleftForward,sensorShortleftRear,dir);
	}

	//left front short long 
	uint8_t left19FAngleK=0;
	int8_t left19FAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortleftForward)&&OK_SENSOR_VALUE(sensorLongleft))
	{
		left19FAngleK=calcKSennsorShort(sensorShortleftForward)*calcKSennsorLong(sensorLongleft);
		left19FAngleDiff=currentAngle-calcSideSensors19(sensorShortleftForward,sensorLongleft,dir);
	}

	//left front short long 
	uint8_t left19RAngleK=0;
	int8_t left19RAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortleftRear)&&OK_SENSOR_VALUE(sensorLongleft))
	{
		left19RAngleK=calcKSennsorShort(sensorShortleftRear)*calcKSennsorLong(sensorLongleft);
		left19RAngleDiff=currentAngle-calcSideSensors19(sensorLongleft,sensorShortleftRear,dir);
	}

	// räkna ut från tidigare vinkelhastighet*tid
	uint8_t vinkelHastighetAngleK=0;
	int8_t vinkelHastighetAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortleftRear)&&OK_SENSOR_VALUE(sensorLongleft))
	{
		vinkelHastighetAngleK=calcKvinkelHastighet(omega);
		vinkelHastighetAngleDiff=currentAngle-omega*T;//TODO
	}
	uint8_t irSensorK = calcKirSensorK(currentAngle);

	int16_t taljare = irSensorK*(frontAngleK*frontAngleDiff + middleAngleK*middleAngleDiff + backAngleK*backAngleDiff + right36AngleDiff*right36AngleK + right19FAngleDiff*right19FAngleK +  right19RAngleDiff*right19RAngleK + left36AngleDiff*left36AngleK + left19FAngleDiff*left19FAngleK +  left19RAngleDiff*left19RAngleK) + vinkelHastighetAngleDiff*vinkelHastighetAngleK;
	int16_t namnare = irSensorK*(frontAngleK + middleAngleK + backAngleK + right36AngleK + right19FAngleK + right19rAngleK + left36AngleK + left19FAngleK + left19rAngleK) + vinkelHastighetAngleK;

	int16_t newAngleDiff = taljare/namnare;
	newAngle=currentAngle+newAngleDiff;
}

//max vikt 5, 5*5*90*9=20250 dvs inte risk för overflow
uint8_t calcKSennsorShort(uint8_t a)
{
	// 10-80 cm * 2 pga konstant i sensorenheten 80 / 4 = 20
	//10/4= 2.5 
	// 20-2.5 = 17.5
	// 17.5/2= 8.75
	// 8.75/2= 4.375 ca 5  dvs max 5 ut.
	return (20-(a>>3))>>2; //kom på något bra för att straffa om värdet är lågt.
}

uint8_t calcKSennsorLong(uint8_t a)// 20-150 cm 150/8=18.75 CA 20
{
	// 20/8= 2.5
	// 20-2.5 = 17.5
	// 17.5/2= 8.75
	// 8.75/2= 4.375 ca 5 dvs max 5 ut.
	return (20-(a>>3))>>2; //kom på något bra för att straffa om värdet är lågt.
}
uint8_t calcKvinkelHastighet(int8_t omega)
{
	return (omega*omega)>>2;//TODO fixa så ju högre vinkelhastighet, destå högre K
}

uint8_t calcKirSensorK(uint8_t angle)
{
	// 90 / 8 = 11.25
	// 90 / 16 = 5.625 
	return (90-angle)>>4;//TODO ju högre vinkel destå lägre K, fixa så den växer snabbare.
}


int8_t calcOpositeSensors(uint8_t r,uint8_t l, int8_t dir)
{
	//lookup table

	return ;//times dir
}

int8_t calcSideSensors36(uint8_t f,uint8_t b, int8_t dir)
{
	//lookup table

	return ;//times dir
}

int8_t calcSideSensors19(uint8_t f,uint8_t b, int8_t dir)
{
	//lookup table

	return ;//times dir
}
