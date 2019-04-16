/* class for the camera in the scene */
class Camera
{
public:
	// constructor
	Camera(Vec3 lookFrom, Vec3 wUp, float y, float p, float fieldOfView, float aspectRatio)
		: camOrigin(lookFrom), worldUp(wUp), yaw(y), pitch(p), FoV(fieldOfView), aR(aspectRatio) { UpdateCamera(); }

	// updates the camera
	void UpdateCamera()
	{
		// update the screen space and the camera space
		UpdateScreenSpace();
		UpdateCameraSpace();

		// update the axis position data
		axesOrigin = camOrigin - (right * halfWidth) - (up * halfHeight) - front;
		horizontal = right * 2.0f * halfWidth;
		vertical = up * 2.0f * halfHeight;
	}

	// variables
	float yaw, pitch;
	float FoV, aR;

	// front, right and up, relative to camera space
	Vec3 front, right, up;

	// position of the camera
	Vec3 camOrigin;

private:
	// screenspace variables
	Vec3 axesOrigin, horizontal, vertical, worldUp;

	// half the width and height of the screenspace
	float halfHeight, halfWidth;

	// update the screen space
	void UpdateScreenSpace()
	{
		halfHeight = tan(ToRadians(FoV) / 2.0f);
		halfWidth = aR * halfHeight;
	}

	// update the camera space
	void UpdateCameraSpace()
	{
		// pre calculate variables
		Vec3 f;
		float radYaw = ToRadians(yaw), radPitch = ToRadians(pitch);

		// calculate the new front vector based on the yaw and pitch
		f.f[0] = cos(radYaw) * cos(radPitch);
		f.f[1] = sin(radPitch);
		f.f[2] = sin(radYaw) * cos(radPitch);
		front = f.Normalized();

		// calculate the other vectors by taking the perpendiculars
		right = (worldUp.Cross(front)).Normalized();
		up = (front.Cross(right)).Normalized();
	}

	// convert degrees to radians
	float ToRadians(float degrees) { return degrees * PI / 180.0f; }
};