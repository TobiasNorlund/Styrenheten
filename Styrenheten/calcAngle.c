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
	uint8_t frontAngle=0;
	if(OK_SENSOR_VALUE(sensorShortLeftForward)&&OK_SENSOR_VALUE(sensorShortRightForward))
	{
		frontAngleK=calcKSennsorShort(sensorShortLeftForward)*calcKSennsorShort(sensorShortRightForward);
		frontAngle=calcOpositeSensors(sensorShortRightForward,sensorShortLeftForward,dir);
	}

	//middle
	uint8_t middleAngleK=0;
	uint8_t middleAngle=0;
	if(OK_SENSOR_VALUE(sensorLongLeft)&&OK_SENSOR_VALUE(sensorLongRight))
	{
		middleAngleK=calcKSennsorLong(sensorLongLeft)*calcKSennsorLong(sensorLongRight);
		middleAngle=calcOpositeSensors(sensorLongRight,sensorLongLeft,dir);
	}
	//back
	uint8_t backAngleK=0;
	uint8_t backAngle=0;
	if(OK_SENSOR_VALUE(sensorShortLeftRear)&&OK_SENSOR_VALUE(sensorShortRightRear))
	{
		backAngleK=calcKSennsorShort(sensorShortLeftRear)*calcKSennsorShort(sensorShortRightRear);
		backAngle=calcOpositeSensors(sensorShortRightRear,sensorShortLeftRear,dir);
	}
	//right 2 short
	uint8_t right36AngleK=0;
	uint8_t right36Angle=0;
	if(OK_SENSOR_VALUE(sensorShortRightForward)&&OK_SENSOR_VALUE(sensorShortRightRear))
	{
		right36AngleK=calcKSennsorShort(sensorShortRightForward)*calcKSennsorShort(sensorShortRightRear);
		right36Angle=calcSideSensors36(sensorShortRightForward,sensorShortRightRear,dir);
	}

	//right front short long 
	uint8_t right19FAngleK=0;
	uint8_t right19FAngle=0;
	if(OK_SENSOR_VALUE(sensorShortRightForward)&&OK_SENSOR_VALUE(sensorLongRight))
	{
		right19FAngleK=calcKSennsorShort(sensorShortRightForward)*calcKSennsorLong(sensorLongRight);
		right19FAngle=calcSideSensors19(sensorShortRightForward,sensorLongRight,dir);
	}

	//right front short long 
	uint8_t right19RAngleK=0;
	uint8_t right19RAngle=0;
	if(OK_SENSOR_VALUE(sensorShortRightRear)&&OK_SENSOR_VALUE(sensorLongRight))
	{
		right19RAngleK=calcKSennsorShort(sensorShortRightRear)*calcKSennsorLong(sensorLongRight);
		right19RAngle=calcSideSensors19(sensorLongRight,sensorShortRightRear,dir);
	}

	//left 2 short
	uint8_t left36AngleK=0;
	uint8_t left36Angle=0;
	if(OK_SENSOR_VALUE(sensorShortleftForward)&&OK_SENSOR_VALUE(sensorShortleftRear))
	{
		left36AngleK=calcKSennsorShort(sensorShortleftForward)*calcKSennsorShort(sensorShortleftRear);
		left36Angle=calcSideSensors36(sensorShortleftForward,sensorShortleftRear,dir);
	}

	//left front short long 
	uint8_t left19FAngleK=0;
	uint8_t left19FAngle=0;
	if(OK_SENSOR_VALUE(sensorShortleftForward)&&OK_SENSOR_VALUE(sensorLongleft))
	{
		left19FAngleK=calcKSennsorShort(sensorShortleftForward)*calcKSennsorLong(sensorLongleft);
		left19FAngle=calcSideSensors19(sensorShortleftForward,sensorLongleft,dir);
	}

	//left front short long 
	uint8_t left19RAngleK=0;
	uint8_t left19RAngle=0;
	if(OK_SENSOR_VALUE(sensorShortleftRear)&&OK_SENSOR_VALUE(sensorLongleft))
	{
		left19RAngleK=calcKSennsorShort(sensorShortleftRear)*calcKSennsorLong(sensorLongleft);
		left19RAngle=calcSideSensors19(sensorLongleft,sensorShortleftRear,dir);
	}

	//TODO räkna ut från tidigare vinkelhastighet*tid

	uint16_t taljare = frontAngleK*frontAngle + middleAngleK*middleAngle + backAngleK*backAngle + right36AngleK*right36AngleK + right19FAngle*right19FAngleK +  right19RAngle*right19RAngleK + left36AngleK*left36AngleK + left19FAngle*left19FAngleK +  left19RAngle*left19RAngleK;
	uint16_t namnare = frontAngleK + middleAngleK + backAngleK + right36AngleK + right19FAngle + right19rAngle + left36AngleK + left19FAngle + left19rAngle;

	uint16_t newAngle = taljare/namnare;

}
uint8_t calcKSennsorShort(uint8_t a)// 10-80 cm * 2 pga konstant i sensorenheten 80 / 4 = 20
{
	return 20-(a>>3); //kom på något bra för att straffa om värdet är lågt.
}

uint8_t calcKSennsorLong(uint8_t a)// 20-150 cm 150/8=18.75 CA 20
{
	return 20-(a>>3); //kom på något bra för att straffa om värdet är lågt.
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




