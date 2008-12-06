
Trackbar(char windowName, int maxVal, int defVal = 0)
{
	value = defVal;

	static int n = 0;
	n++;

	cvCreateTrackbar((char)n, windowName, &value, maxVal, NULL);


}
