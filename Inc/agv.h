#ifndef AGV_H_INCLUDED
#define AGV_H_INCLUDED
class agv
{
private:
    float _allignDistance;

public:
    agv(float allignDistance);
    ~agv();
    //turnAngle will simply turn the avg the passed angle (in radians).
    void turnAngle(float radians);
    //turnAngleGrap will turn the avg and look for a guide line inside the interval specified by
    //(´radians´ - ´searchAngle´, ´radians´ + ´searchAngle´). Returns false if no guide line was found
    //inside the interval, in which case the avg will make sure to turn exactly radians before returning
    bool turnAngleGrab(float radians, float searchAngle);
    //turn lines will spin while counting how many lines crossed the sensors, stoping once ´lines´ is
    //reached. Returns false once a full rotation is completed but couldn't find sufficient guide lines,
    //in which case the avg makes sure to stop at the initial angle
    bool turnLines(int lines);
    //followLine will attempt to follow the line that's currently between the sensors, returning true once
    //a crossing has been reached. Return false means it was not possible to follow the path to its end,
    //meaning the path is blocked. By default once a crossing is reached the agv will attempt to position
    //itself right above the crossing by advancing the distance specified in `alignDistance`
    bool followLine();
    //hardForward will attempt to advance the specified distance. If the path is block it'll wait until
    //it's unblocked to be able to clear the specified distance
    void hardForward(float distance);
};

#endif