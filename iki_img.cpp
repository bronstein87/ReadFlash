#include "iki_img.h"

const string IKI_img::BoolToString(bool b)
{
	return b ? "true" : "false";
}
// ---------------------------------------------------------------------------

const bool IKI_img::StringToBool(string b)
{
	return (strcmp(b.c_str(), "true") == 0) ? true : false;
}
// ---------------------------------------------------------------------------

string IKI_img::ArcsecToStr(double arcsec)
{
	int deg, min, sec, msc, rem, arc;
	double arf;
	char * st_buf = new char[100];
	string str;
	arf = 1000.0 * arcsec; // in msec
	arc = ROUND(arf);
	arc = abs(arc);
	deg = arc / 3600000; rem = arc % 3600000;
	min = rem / 60000; sec = (rem - min * 60000) / 1000;
	msc = arc - deg * 3600000 - min * 60000 - sec * 1000;
	if (arf >= 0)
		sprintf(st_buf, "%d\°%02d'%02d\".%03d", deg, min, sec, msc);
	else
		sprintf(st_buf, "-%d\°%02d'%02d\".%03d", deg, min, sec, msc);
	str = st_buf;
	delete[]st_buf;
	return str;
}
// ---------------------------------------------------------------------------

double IKI_img::StrToArcsec(string _str)
{
	string _strbuf;
	double arcsec;
	bool negative = false;
	int deg, min, sec, msc, rem, arc;
	int pos_index;
	if ((pos_index = _str.find("{")) >= 0)
		_str = _str.substr(pos_index + 1, _str.size());
	else
		if ((pos_index = _str.find("}")) >= 0)
			_str = _str.substr(0, pos_index);
	// проверка на отрицательность, если есть "-", его удаляем, в конце полученное значение умножаем на -1
	if ((pos_index = _str.find("-")) >= 0)
	{
		_str = _str.substr(pos_index + 1, _str.size());
		negative = true;
	}
	pos_index = _str.find("\°"); _strbuf = _str.substr(0, pos_index);
	deg = atoi(_strbuf.c_str()); _str = _str.substr(pos_index + 1, _str.size() - pos_index);
	pos_index = _str.find("'"); _strbuf = _str.substr(0, pos_index);
	min = atoi(_strbuf.c_str()); _str = _str.substr(pos_index + 1, _str.size() - pos_index);
	pos_index = _str.find("\""); _strbuf = _str.substr(0, pos_index);
	sec = atoi(_strbuf.c_str()); _str = _str.substr(pos_index + 1, _str.size() - pos_index);
	pos_index = _str.find("."); _str = _str.substr(pos_index + 1, _str.size());
	msc = atoi(_str.c_str());
	arcsec = (double)(deg * 3600000 + min * 60000 + sec * 1000 + msc) / (double)1000.0;
	if (negative)
		arcsec *= -1;
	return arcsec;
}
// ---------------------------------------------------------------------------

void IKI_img::StringToFloatMass(double(*mass), unsigned int N, string _str)
{
	int pos_index = 0;
	int pos_C = 0;
	string _strbuf;
	for (unsigned int i = 0; i < N; i++)
		if ((pos_index = _str.find(" ")) >= 0)
		{
			_strbuf = _str.substr(0, pos_index);
			if ((pos_C = _strbuf.find("{")) >= 0)
				_strbuf = _strbuf.substr(pos_C + 1, _strbuf.size() - pos_C); // - pos_C !!!!!!!!!!!!!!!!!!!!!!
			else
				if ((pos_C = _strbuf.find("}")) >= 0)
					_strbuf = _strbuf.substr(0, pos_C);
			mass[i] = atof(_strbuf.c_str());
			_str = _str.substr(pos_index + 1, _str.size() - pos_index);
		}
		else
		{
			if ((pos_C = _str.find("}")) >= 0)
				_str = _str.substr(0, pos_C);
			mass[i] = atof(_str.c_str());
			break;
		}
}
// ---------------------------------------------------------------------------

void IKI_img::StringToFloatMass3D(double(*mas3d)[3], unsigned int N, unsigned int M, string _str)
{
	int pos_index = 0;
	int pos_C = 0;
	string _strbuf;
	for (unsigned int i = 0; i < N; i++)
		for (unsigned int j = 0; j < M; j++)
			if ((pos_index = _str.find(" ")) >= 0)
			{
				_strbuf = _str.substr(0, pos_index);
				if ((pos_C = _strbuf.find("{")) >= 0)
					_strbuf = _strbuf.substr(pos_C + 1, _strbuf.size());
				else
					if ((pos_C = _strbuf.find("}")) >= 0)
						_strbuf = _strbuf.substr(0, pos_C);
				mas3d[i][j] = atof(_strbuf.c_str());
				_str = _str.substr(pos_index + 1, _str.size() - pos_index);
			}
			else
			{
				if ((pos_C = _str.find("}")) >= 0)
					_str = _str.substr(0, pos_C);
				mas3d[i][j] = atof(_str.c_str());
				break;
			}
}
// ---------------------------------------------------------------------------

IKI_img::IKI_img(void)
{
	memset(&Georeferencing, 0, sizeof(Georeferencing));
	memset(&CameraSettings, 0, sizeof(CameraSettings));
	memset(&FilterData, 0, sizeof(FilterData));
	memset(&StarsData, 0, sizeof(StarsData));
	memset(&ImageData, 0, sizeof(ImageData));
	Georeferencing.SoftwareVersion = VERSION_ID;
}
// ---------------------------------------------------------------------------

IKI_img::~IKI_img()
{
	_FreeMemoryArray(CameraSettings.DistortionFunction.Coefficient);
	_FreeMemoryArray(CameraSettings.TableDeadObjects.PixelData);
	_FreeMemoryArray(FilterData.DataPixels);
	_FreeMemoryArray(StarsData.StarsList);
	_FreeMemoryArray(StarsData.SimulatedFrame.StarRec);
	_FreeMemoryArray(ImageData.FrameData.Data);
	_FreeMemoryArray(ImageData.LinesData.Info);
	_FreeMemoryArray(ImageData.LinesData.Data);
	_FreeMemoryArray(ImageData.WindowsData.Info);
	_FreeMemoryArray(ImageData.WindowsData.Data);
	memset(&Georeferencing, 0, sizeof(Georeferencing));
	memset(&CameraSettings, 0, sizeof(CameraSettings));
	memset(&FilterData, 0, sizeof(FilterData));
	memset(&StarsData, 0, sizeof(StarsData));
	memset(&ImageData, 0, sizeof(ImageData));
}
// ---------------------------------------------------------------------------

void IKI_img::print_string(FILE* FStream, unsigned int ID, string name, string value)
{
	char buffer_string[4];
	unsigned int sizedata = value.size();
	memmove(&buffer_string[0], &ID, 4); fwrite(&buffer_string[0], 1, 4, FStream);
	fprintf(FStream, "%-80s", name.data());
	memmove(&buffer_string[0], &sizedata, 4); fwrite(&buffer_string[0], 1, 4, FStream);
	fprintf(FStream, "%s\r\n", value.data());
	fflush(FStream);
}
// ---------------------------------------------------------------------------

void IKI_img::print_data(FILE* FStream, string value)
{
	char buffer_string[4];
	unsigned int sizedata = value.size();
	memmove(&buffer_string[0], &sizedata, 4); fwrite(&buffer_string[0], 1, 4, FStream);
	fprintf(FStream, "%s\r\n", value.data());
}
// ---------------------------------------------------------------------------

void IKI_img::WriteFormat(string filename) // define_format_value = "%4x%-80.80s%4x%s\r\n"
{
	unsigned int sizedata = 0;
	char buffer_string[200] = {0};
	FILE* FStream = fopen(filename.c_str(), "wb+");
#pragma region Georeferencing
	print_string(FStream, datetime_id, datetime_name, Georeferencing.DateTime);
	print_string(FStream, soft_vers_id, soft_vers_name, Georeferencing.SoftwareVersion);
	print_string(FStream, frame_num_id, frame_num_name, toString(Georeferencing.FrameNumber));
	print_string(FStream, protons_density_id, protons_density_name, toString(Georeferencing.ProtonsDensity));
	print_string(FStream, tracks_brightness_id, tracks_brightness_name, toString(Georeferencing.TracksBrightness));
	if (Georeferencing.MoonVelocity != 0)
		print_string(FStream, moon_velocity_id, moon_velocity_name, toString(Georeferencing.MoonVelocity));
	if (Georeferencing.MoonBrightness != 0)
		print_string(FStream, moon_brightness_id, moon_brightness_name, toString(Georeferencing.MoonBrightness));
	print_string(FStream, bright_left_id, bright_left_name, toString(Georeferencing.BrightnessLeft));
	print_string(FStream, bright_right_id, bright_right_name, toString(Georeferencing.BrightnessRight));
	print_string(FStream, era_id, era_name, FormatFloatTo_string("%8.6f", Georeferencing.Era));
	print_string(FStream, orbit_eccentricity_id, orbit_eccentricity_name, FormatFloatTo_string("%.3f", Georeferencing.OrbitEccentricity));
	print_string(FStream, length_semiaxis_id, length_semiaxis_name, FormatFloatTo_string("%.3f", Georeferencing.LengthSemiaxis));
	print_string(FStream, length_radius_vector_id, length_radius_vector_name, FormatFloatTo_string("%.3f", Georeferencing.LengthRadiusVector));
	print_string(FStream, longitude_node_id, longitude_node_name, ArcsecToStr(Georeferencing.LongitudeAscendingNode));
	print_string(FStream, inclination_orbit_id, inclination_orbit_name, ArcsecToStr(Georeferencing.InclinationOrbit));
	print_string(FStream, argument_perigee_id, argument_perigee_name, ArcsecToStr(Georeferencing.ArgumentPerigee));
	print_string(FStream, argument_latitude_id, argument_latitude_name, ArcsecToStr(Georeferencing.ArgumentLatitude));
	memset(&buffer_string[0], 0, 200);
	sprintf(buffer_string, "%12.10lf %12.10lf %12.10lf", Georeferencing.DeviceAngularVelocity[0], Georeferencing.DeviceAngularVelocity[1], Georeferencing.DeviceAngularVelocity[2]);
	print_string(FStream, dev_angular_velocity_id, dev_angular_velocity_name, (const char*)buffer_string);
	memset(&buffer_string[0], 0, 200);
	sprintf(buffer_string, "%12.10lf %12.10lf %12.10lf", Georeferencing.DeviceAngularAcceleration[0], Georeferencing.DeviceAngularAcceleration[1], Georeferencing.DeviceAngularAcceleration[2]);
	print_string(FStream, dev_angular_acceler_id, dev_angular_acceler_name, (const char*)buffer_string);
	memset(&buffer_string[0], 0, 200);
	sprintf(buffer_string, "%12.10lf %12.10lf %12.10lf", Georeferencing.ProjectionLinearVelocity[0], Georeferencing.ProjectionLinearVelocity[1], Georeferencing.ProjectionLinearVelocity[2]);
	print_string(FStream, proj_line_velo_id, proj_line_velo_name, (const char*)buffer_string);
	memset(&buffer_string[0], 0, 200);
	sprintf(buffer_string, "%12.10lf %12.10lf %12.10lf", Georeferencing.OrientationAngles[0], Georeferencing.OrientationAngles[1], Georeferencing.OrientationAngles[2]);
	print_string(FStream, type_angle_id, type_angle_name, (const char*)buffer_string);
	if (Georeferencing.OrientationQuaternion[0] != 0 || Georeferencing.OrientationQuaternion[1] != 0 || Georeferencing.OrientationQuaternion[2] != 0 || Georeferencing.OrientationQuaternion[3] != 0)
	{
		memset(&buffer_string[0], 0, 200);
		sprintf(buffer_string, "%12.10lf %12.10lf %12.10lf %12.10lf", Georeferencing.OrientationQuaternion[0], Georeferencing.OrientationQuaternion[1], Georeferencing.OrientationQuaternion[2], Georeferencing.OrientationQuaternion[3]);
		print_string(FStream, type_quat_id, type_quat_name, (const char*)buffer_string);
	}
	memset(&buffer_string[0], 0, 200);
	sprintf(buffer_string, "{%12.10lf %12.10lf %12.10lf} {%12.10lf %12.10lf %12.10lf} {%12.10lf %12.10lf %12.10lf}",
		Georeferencing.OrientationMatrix[0][0], Georeferencing.OrientationMatrix[0][1], Georeferencing.OrientationMatrix[0][2],
		Georeferencing.OrientationMatrix[1][0], Georeferencing.OrientationMatrix[1][1], Georeferencing.OrientationMatrix[1][2],
		Georeferencing.OrientationMatrix[2][0], Georeferencing.OrientationMatrix[2][1], Georeferencing.OrientationMatrix[2][2]);
	print_string(FStream, type_matr_id, type_matr_name, (const char*)buffer_string);
	print_string(FStream, year_aber_app_id, year_aber_app_name, BoolToString(Georeferencing.YearAberrationApply));
	print_string(FStream, satel_aber_app_id, satel_aber_app_name, BoolToString(Georeferencing.SatelliteAberrationApply));
	print_string(FStream, preces_nutat_app_id, preces_nutat_app_name, BoolToString(Georeferencing.PrecessionNutationApply));
	print_string(FStream, photon_noise_app_id, photon_noise_app_name, BoolToString(Georeferencing.PhotonNoiseApply));
	print_string(FStream, distortion_app_id, distortion_app_name, BoolToString(Georeferencing.DistortionApply));
#pragma end_region

#pragma region CameraSettings
	if (CameraSettings.DataSource != "")
		print_string(FStream, device_type_id, device_type_name, CameraSettings.DataSource);
	if (CameraSettings.SNDevice != 0)
		print_string(FStream, device_sn_id, device_sn_name, toString(CameraSettings.SNDevice));
	print_string(FStream, inner_radius_id, inner_radius_name, toString(CameraSettings.InnerRadius));
	print_string(FStream, outer_radius_id, outer_radius_name, toString(CameraSettings.OuterRadius));
	if (CameraSettings.ResolutionACP)
		print_string(FStream, resolution_acp_id, resolution_acp_name, toString(CameraSettings.ResolutionACP));
	if (CameraSettings.Frequency != 0)
		print_string(FStream, frequency_id, frequency_name, toString(CameraSettings.Frequency));
	if (CameraSettings.EV_ACP != 0)
		print_string(FStream, ev_acp_id, ev_acp_name, FormatFloatTo_string("%8.3f", CameraSettings.EV_ACP));
	if (CameraSettings.PixelSize != 0)
		print_string(FStream, pixel_size_id, pixel_size_name, FormatFloatTo_string("%8.6f", CameraSettings.PixelSize));
	if (CameraSettings.FocalLength != 0)
		print_string(FStream, focal_length_id, focal_length_name, FormatFloatTo_string("%8.3f", CameraSettings.FocalLength));
	if (CameraSettings.DiameterEntrancePupil != 0)
		print_string(FStream, diametr_entrace_pupil_id, diametr_entrace_pupil_name, FormatFloatTo_string("%8.3f", CameraSettings.DiameterEntrancePupil));
	print_string(FStream, dark_level_id, dark_level_name, FormatFloatTo_string("%8.3f", CameraSettings.DarkLevel));
	print_string(FStream, dark_sko_id, dark_sko_name, FormatFloatTo_string("%8.3f", CameraSettings.DarkSKO));
	if (CameraSettings.LimitingMagnitude != 0)
		print_string(FStream, limit_magn_id, limit_magn_name, FormatFloatTo_string("%8.6f", CameraSettings.LimitingMagnitude));
	if (CameraSettings.StarsDistribution != 0)
		print_string(FStream, stars_dist_id, stars_dist_name, FormatFloatTo_string("%8.3f", CameraSettings.StarsDistribution));
	if (Georeferencing.DistortionApply)
	{
		memset(&buffer_string[0], 0, 200);
		sprintf(buffer_string, "%u %u", CameraSettings.DistortionFunction.PolynomialDegree, CameraSettings.DistortionFunction.CoefficientsCount);
		print_string(FStream, function_distortion_id, function_distortion_name, (const char*)buffer_string);
		for (int i = 0; i < CameraSettings.DistortionFunction.CoefficientsCount; i++)
		{
			memset(&buffer_string[0], 0, 200);
			sprintf(buffer_string, "%12.10f %12.10f", CameraSettings.DistortionFunction.Coefficient[i].X, CameraSettings.DistortionFunction.Coefficient[i].Y);
			print_data(FStream, (const char*)buffer_string);
		}
	}
	if (CameraSettings.TableDeadObjects.RecordsCount != 0)
	{
		print_string(FStream, dead_pix_id, dead_pix_name, toString(CameraSettings.TableDeadObjects.RecordsCount));
		for (int i = 0; i < CameraSettings.TableDeadObjects.RecordsCount; i++)
		{
			memset(&buffer_string[0], 0, 200);
			sprintf(buffer_string, "%8.6f %8.6f %8.6f %u", CameraSettings.TableDeadObjects.PixelData[i].X_coordinate, CameraSettings.TableDeadObjects.PixelData[i].Y_coordinate, CameraSettings.TableDeadObjects.PixelData[i].BrightnessObj, CameraSettings.TableDeadObjects.PixelData[i].CoefficientObj);
			print_data(FStream, (const char*)buffer_string);
		}
	}
	print_string(FStream, matrixtemp_id, matrixtemp_name, FormatFloatTo_string("%10.7f", CameraSettings.MatrixTemp));
#pragma end_region

#pragma region FilterData
	if (FilterData.FilteredPixelsCount != 0)
	{
		print_string(FStream, bottomright_id, bottomright_name, BoolToString(FilterData.BottomRight));
		memset(&buffer_string[0], 0, 200);
		sprintf(buffer_string, "%u %s", FilterData.FilteredPixelsCount, FilterData.TagFilter.data());
		print_string(FStream, filter_pix_id, filter_pix_name, (const char*)buffer_string);
		for (int i = 0; i < FilterData.FilteredPixelsCount; i++)
		{
			memset(&buffer_string[0], 0, 200);
			sprintf(buffer_string, "%u %u %u", FilterData.DataPixels[i].X_coordinate, FilterData.DataPixels[i].Y_coordinate, FilterData.DataPixels[i].BrightnessPixel);
			print_data(FStream, (const char*)buffer_string);
		}
	}
#pragma end_region

#pragma region starsinfo
	print_string(FStream, rezstat_id, rezstat_name, toString(StarsData.RezStat));
	print_string(FStream, epsilon_id, epsilon_name, FormatFloatTo_string("%8.6f", StarsData.Epsilon));
	if (StarsData.m_X != 0 || StarsData.m_Y != 0 || StarsData.m_Cur != 0)
	{
		memset(&buffer_string[0], 0, 200);
		sprintf(buffer_string, "8.6f %8.6f %8.6f", StarsData.m_X, StarsData.m_Y, StarsData.m_Cur);
		print_string(FStream, m_X_Y_Cyr_id, m_X_Y_Cyr_name, (const char*)buffer_string);
	}
	print_string(FStream, local_recog_id, local_recog_name, toString(StarsData.LocalizedCount));
	if (StarsData.SizeLocalList != 0)
	{
		memset(&buffer_string[0], 0, 200);
		sprintf(buffer_string, "%d %d", StarsData.SizeLocalList, StarsData.RecognizedCount);
		print_string(FStream, sizelocallist_id, sizelocallist_name, (const char*)buffer_string);
		char str[80];
		for (int i = 0; i < StarsData.SizeLocalList; i++)
		{
			memset(&buffer_string[0], 0, 200);
			sprintf(buffer_string, "%8.6f %8.6f %8.6f %8.6f ", StarsData.StarsList[i].BrightnessObject, StarsData.StarsList[i].PixelsCount, StarsData.StarsList[i].X_coordinate, StarsData.StarsList[i].Y_coordinate);
			if (StarsData.StarsList[i].NumberStar == 0)
				goto endlist;
			else
			{
				sprintf(str, "%u %2.1f %c%c %8.6f %8.6f", StarsData.StarsList[i].NumberStar, StarsData.StarsList[i].StellarMagnitude, StarsData.StarsList[i].SpectralClass[0], StarsData.StarsList[i].SpectralClass[1], StarsData.StarsList[i].DX, StarsData.StarsList[i].DY);
				strcat(buffer_string, str);
			}
		endlist: print_data(FStream, (const char*)buffer_string);
		}
	}
	print_string(FStream, rec_id, rec_name, toString(StarsData.SimulatedFrame.strrec));
	if (StarsData.SimulatedFrame.SizeStarList != 0)
	{
		print_string(FStream, sizestarlist_id, sizestarlist_name, toString(StarsData.SimulatedFrame.SizeStarList));
		for (int i = 0; i < StarsData.SimulatedFrame.SizeStarList; i++)
		{
			memset(&buffer_string[0], 0, 200);
			sprintf(buffer_string, "%u %lu %8.6f %8.6f %2.1f %c%c", StarsData.SimulatedFrame.StarRec[i].Ns, StarsData.SimulatedFrame.StarRec[i].Is, StarsData.SimulatedFrame.StarRec[i].Xs, StarsData.SimulatedFrame.StarRec[i].Ys, StarsData.SimulatedFrame.StarRec[i].Mv, StarsData.SimulatedFrame.StarRec[i].Sp[0], StarsData.SimulatedFrame.StarRec[i].Sp[1]);
			print_data(FStream, (const char*)buffer_string);
		}
	}
	if (StarsData.RecognizedOrientationAngles[0] != 0 || StarsData.RecognizedOrientationAngles[1] != 0 || StarsData.RecognizedOrientationAngles[2] != 0)
	{
		memset(&buffer_string[0], 0, 200);
		sprintf(buffer_string, "12.10f %12.10f %12.10f", StarsData.RecognizedOrientationAngles[0], StarsData.RecognizedOrientationAngles[1], StarsData.RecognizedOrientationAngles[2]);
		print_string(FStream, rec_orient_angl_id, rec_orient_angl_name, (const char*)buffer_string);
	}
	if (StarsData.RecognizedOrientationQuaternion[0] != 0 || StarsData.RecognizedOrientationQuaternion[1] != 0 || StarsData.RecognizedOrientationQuaternion[2] != 0 || StarsData.RecognizedOrientationQuaternion[3] != 0)
	{
		memset(&buffer_string[0], 0, 200);
		sprintf(buffer_string, "%12.10f %12.10f %12.10f %12.10f", StarsData.RecognizedOrientationQuaternion[0], StarsData.RecognizedOrientationQuaternion[1], StarsData.RecognizedOrientationQuaternion[2], StarsData.RecognizedOrientationQuaternion[3]);
		print_string(FStream, rec_orient_quat_id, rec_orient_quat_name, (const char*)buffer_string);
	}
	if (StarsData.RecognizedOrientationMatrix[0][0] != 0 || StarsData.RecognizedOrientationMatrix[0][1] != 0 || StarsData.RecognizedOrientationMatrix[0][2] != 0 ||
		StarsData.RecognizedOrientationMatrix[1][0] != 0 || StarsData.RecognizedOrientationMatrix[1][1] != 0 || StarsData.RecognizedOrientationMatrix[1][2] != 0 ||
		StarsData.RecognizedOrientationMatrix[2][0] != 0 || StarsData.RecognizedOrientationMatrix[2][1] != 0 || StarsData.RecognizedOrientationMatrix[2][2] != 0)
	{
		memset(&buffer_string[0], 0, 200);
		sprintf(buffer_string, "{%12.10f %12.10f %12.10f} {%12.10f %12.10f %12.10f} {%12.10f %12.10f %12.10f}",
			StarsData.RecognizedOrientationMatrix[0][0], StarsData.RecognizedOrientationMatrix[0][1], StarsData.RecognizedOrientationMatrix[0][2],
			StarsData.RecognizedOrientationMatrix[1][0], StarsData.RecognizedOrientationMatrix[1][1], StarsData.RecognizedOrientationMatrix[1][2],
			StarsData.RecognizedOrientationMatrix[2][0], StarsData.RecognizedOrientationMatrix[2][1], StarsData.RecognizedOrientationMatrix[2][2]);
		print_string(FStream, rec_orient_matr_id, rec_orient_matr_name, (const char*)buffer_string);
	}
	if (StarsData.RecognizedAngularVelocity[0] != 0 || StarsData.RecognizedAngularVelocity[1] != 0 || StarsData.RecognizedAngularVelocity[2] != 0)
	{
		memset(&buffer_string[0], 0, 200);
		sprintf(buffer_string, "%12.10f %12.10f %12.10f", StarsData.RecognizedAngularVelocity[0], StarsData.RecognizedAngularVelocity[1], StarsData.RecognizedAngularVelocity[2]);
		print_string(FStream, rec_angular_vel_id, rec_angular_vel_name, (const char*)buffer_string);
	}
#pragma end_region

#pragma region ImageData
	print_string(FStream, exp_time_id, exp_time, FormatFloatTo_string("%8.6f", ImageData.ExposureTime));
	if (ImageData.FrameData.FrameWidth != 0 && ImageData.FrameData.FrameHeight != 0)
	{
		print_string(FStream, stringframe_type_id, stringframe_type, toString(ImageData.FrameData.DataType));
		print_string(FStream, stringframe_deg_bin_id, stringframe_deg_bin, toString(ImageData.FrameData.DegreeBinning));
		print_string(FStream, stringframe_width_id, stringframe_width, toString(ImageData.FrameData.FrameWidth));
		print_string(FStream, stringframe_height_id, stringframe_height, toString(ImageData.FrameData.FrameHeight));
		print_string(FStream, stringframe_SizeData_id, stringframe_SizeData, toString(ImageData.FrameData.SizeData));
	}
	if (ImageData.LinesData.LinesCount != 0)
	{
		print_string(FStream, stringlines_count_id, stringlines_count, toString(ImageData.LinesData.LinesCount));
		print_string(FStream, stringlines_type_id, stringlines_type, toString(ImageData.LinesData.DataType));
		print_string(FStream, stringlines_deg_bin_id, stringlines_deg_bin, toString(ImageData.LinesData.DegreeBinning));
		print_string(FStream, stringlines_width_id, stringlines_width, toString(ImageData.LinesData.LinesWidth));
		print_string(FStream, stringlines_height_id, stringlines_height, toString(ImageData.LinesData.LinesHeight));
		print_string(FStream, stringlines_SizeData_id, stringlines_SizeData, toString(ImageData.LinesData.SizeData));
		print_string(FStream, stringlines_info_id, stringlines_info, "");
		for (int i = 0; i < ImageData.LinesData.LinesCount; i++)
		{
			memset(&buffer_string[0], 0, 200);
			sprintf(buffer_string, "%d %d", ImageData.LinesData.Info[i].Y_FirstString, ImageData.LinesData.Info[i].CountString);
			print_data(FStream, (const char*)buffer_string);
		}
	}
	if (ImageData.WindowsData.SizeWindowList != 0)
	{
		print_string(FStream, stringwindow_count_id, stringwindow_count, toString(ImageData.WindowsData.WindowCount));
		print_string(FStream, stringwindow_sizewindowlist_id, stringwindow_sizewindowlist, toString(ImageData.WindowsData.SizeWindowList));
		print_string(FStream, stringwindow_type_id, stringwindow_type, toString(ImageData.WindowsData.DataType));
		print_string(FStream, stringwindow_deg_bin_id, stringwindow_deg_bin, toString(ImageData.WindowsData.DegreeBinning));
		print_string(FStream, stringwindow_SizeData_id, stringwindow_SizeData, toString(ImageData.WindowsData.SizeData));
		print_string(FStream, stringwindow_flag_id, stringwindow_flag, BoolToString(ImageData.WindowsData.BottomRight));
		print_string(FStream, stringwindow_info_id, stringwindow_info, "");
		for (int i = 0; i < ImageData.WindowsData.SizeWindowList; i++)
		{
			memset(&buffer_string[0], 0, 200);
			sprintf(buffer_string, "%u %u %u %u %u %u %8.6f %8.6f %u %u", ImageData.WindowsData.Info[i].WindowWidth, ImageData.WindowsData.Info[i].WindowHeight, ImageData.WindowsData.Info[i].X, ImageData.WindowsData.Info[i].Y, ImageData.WindowsData.Info[i].Limit, ImageData.WindowsData.Info[i].ObjCount, ImageData.WindowsData.Info[i].SKO, ImageData.WindowsData.Info[i].Average, ImageData.WindowsData.Info[i].ZipX, ImageData.WindowsData.Info[i].ZipY);
			print_data(FStream, (const char*)buffer_string);
		}
	}

	if (ImageData.FrameData.FrameWidth != 0 && ImageData.FrameData.FrameHeight != 0)
	{
		print_string(FStream, videoframe_id, videoframe_name, "");
		switch (ImageData.FrameData.DataType)
		{
		case 0: fwrite((unsigned char*) ImageData.FrameData.Data, MassType[ImageData.FrameData.DataType], ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight, FStream); break;
		case 1: fwrite((unsigned short*) ImageData.FrameData.Data, MassType[ImageData.FrameData.DataType], ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight, FStream); break;
		case 2: fwrite((float*) ImageData.FrameData.Data, MassType[ImageData.FrameData.DataType], ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight, FStream); break;
		}
		fprintf(FStream, "\r\n");
	}

	if (ImageData.LinesData.LinesCount != 0)
	{
		print_string(FStream, videolines_id, videolines_name, "");
		switch (ImageData.LinesData.DataType)
		{
		case 0: fwrite((unsigned char*) ImageData.LinesData.Data, MassType[ImageData.LinesData.DataType], ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight, FStream); break;
		case 1: fwrite((unsigned short*) ImageData.LinesData.Data, MassType[ImageData.LinesData.DataType], ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight, FStream); break;
		case 2: fwrite((float*) ImageData.LinesData.Data, MassType[ImageData.LinesData.DataType], ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight, FStream); break;
		}
		fprintf(FStream, "\r\n");
	}

	if (ImageData.WindowsData.SizeWindowList != 0)
	{
		print_string(FStream, videowindow_id, videowindow_name, "");
		switch (ImageData.WindowsData.DataType)
		{
		case 0: fwrite((unsigned char*) ImageData.WindowsData.Data, MassType[ImageData.WindowsData.DataType], ImageData.WindowsData.SizeData, FStream); break;
		case 1: fwrite((unsigned short*) ImageData.WindowsData.Data, MassType[ImageData.WindowsData.DataType], ImageData.WindowsData.SizeData, FStream); break;
		case 2: fwrite((float*) ImageData.WindowsData.Data, MassType[ImageData.WindowsData.DataType], ImageData.WindowsData.SizeData, FStream); break;
		}
		fprintf(FStream, "\r\n");
	}
#pragma end_region
	fclose(FStream);
}
// ---------------------------------------------------------------------------

void IKI_img::Clear_Memory(void)
{
	_FreeMemoryArray(CameraSettings.DistortionFunction.Coefficient);
	_FreeMemoryArray(CameraSettings.TableDeadObjects.PixelData);
	_FreeMemoryArray(FilterData.DataPixels);
	_FreeMemoryArray(StarsData.StarsList);
	_FreeMemoryArray(StarsData.SimulatedFrame.StarRec);
	_FreeMemoryArray(ImageData.FrameData.Data);
	_FreeMemoryArray(ImageData.LinesData.Info);
	_FreeMemoryArray(ImageData.LinesData.Data);
	_FreeMemoryArray(ImageData.WindowsData.Info);
	_FreeMemoryArray(ImageData.WindowsData.Data);
	memset(&Georeferencing, 0, sizeof(Georeferencing));
	memset(&CameraSettings, 0, sizeof(CameraSettings));
	memset(&FilterData, 0, sizeof(FilterData));
	memset(&StarsData, 0, sizeof(StarsData));
	memset(&ImageData, 0, sizeof(ImageData));
}
// ---------------------------------------------------------------------------

bool IKI_img::ReadFormat(string filename, bool Need_Erase)
{
	int count_find_pos = 0;
	int pos_index = 0;
	unsigned int start = 0;
	unsigned int sizedata = 0;
	string buferstr;
	double buf[10] = {0};
	char buf_str[1000] = {0};
	char* end_str = new char[2];
	char* bufer = new char[80];
	char* Data_buf = new char[10240]; // взял с запасом чтоб каждый раз не выделять и не очищать память под разбор каждой строки
	FILE* FStream = fopen(filename.c_str(), "rb");
	if (Need_Erase) // если true то очищаем перед чтением
			Clear_Memory();
	while (!feof(FStream))
	{
		memset(&buf_str[0], 0, 1000); fread(&buf_str[0], 1, 4, FStream); memmove(&start, &buf_str[0], 4); // ID
		memset(&bufer[0], 0, 80); fread(&bufer[0], 1, 80, FStream); // Name
		memset(&buf_str[0], 0, 1000); fread(&buf_str[0], 1, 4, FStream); memmove(&sizedata, &buf_str[0], 4); // size info
		memset(&Data_buf[0], 0, 10240); fread(&Data_buf[0], 1, sizedata, FStream); // info
		fread(&end_str[0], 1, 2, FStream); // символы конца строки \r\n

		if (end_str[0] != '\r' && end_str[1] != '\n') // вроде контрольной суммы, если в конце не они значит неправильно считана строка
		{
			_FreeMemoryArray(end_str);
			_FreeMemoryArray(bufer);
			_FreeMemoryArray(Data_buf);
			_FreeMemory(FStream);
			return false;
		}
		buferstr = (const char*) Data_buf;
		switch (start)
		{
#pragma region Georeferencing
		case datetime_id: Georeferencing.DateTime = buferstr; break;
		case soft_vers_id: Georeferencing.SoftwareVersion = buferstr; break;
		case frame_num_id: Georeferencing.FrameNumber = atoi(buferstr.c_str()); break;
		case protons_density_id: Georeferencing.ProtonsDensity = atoi(buferstr.c_str()); break;
		case tracks_brightness_id: Georeferencing.TracksBrightness = atoi(buferstr.c_str()); break;
		case moon_velocity_id: Georeferencing.MoonVelocity = atoi(buferstr.c_str()); break;
		case moon_brightness_id: Georeferencing.MoonBrightness = atoi(buferstr.c_str()); break;
		case bright_left_id: Georeferencing.BrightnessLeft = atoi(buferstr.c_str()); break;
		case bright_right_id: Georeferencing.BrightnessRight = atoi(buferstr.c_str()); break;
		case era_id: Georeferencing.Era = (float) atof(buferstr.c_str()); break;
		case orbit_eccentricity_id: Georeferencing.OrbitEccentricity = (double) atof(buferstr.c_str()); break;
		case length_semiaxis_id: Georeferencing.LengthSemiaxis = (double) atof(buferstr.c_str()); break;
		case length_radius_vector_id: Georeferencing.LengthRadiusVector = (double) atof(buferstr.c_str()); break;
		case longitude_node_id: Georeferencing.LongitudeAscendingNode = (double) StrToArcsec(buferstr); break;
		case inclination_orbit_id: Georeferencing.InclinationOrbit = (double) StrToArcsec(buferstr); break;
		case argument_perigee_id: Georeferencing.ArgumentPerigee = (double) StrToArcsec(buferstr); break;
		case argument_latitude_id: Georeferencing.ArgumentLatitude = (double) StrToArcsec(buferstr); break;
		case dev_angular_velocity_id: StringToFloatMass(Georeferencing.DeviceAngularVelocity, 3, buferstr); break;
		case dev_angular_acceler_id: StringToFloatMass(Georeferencing.DeviceAngularAcceleration, 3, buferstr); break;
		case proj_line_velo_id: StringToFloatMass(Georeferencing.ProjectionLinearVelocity, 3, buferstr); break;
		case type_angle_id: StringToFloatMass(Georeferencing.OrientationAngles, 3, buferstr); break;
		case type_quat_id: StringToFloatMass(Georeferencing.OrientationQuaternion, 4, buferstr); break;
		case type_matr_id: StringToFloatMass3D(Georeferencing.OrientationMatrix, 3, 3, buferstr); break;
		case year_aber_app_id: Georeferencing.YearAberrationApply = StringToBool(buferstr); break;
		case satel_aber_app_id: Georeferencing.SatelliteAberrationApply = StringToBool(buferstr); break;
		case preces_nutat_app_id: Georeferencing.PrecessionNutationApply = StringToBool(buferstr); break;
		case photon_noise_app_id: Georeferencing.PhotonNoiseApply = StringToBool(buferstr); break;
		case distortion_app_id: Georeferencing.DistortionApply = StringToBool(buferstr); break;
#pragma end_region
#pragma region CameraSettings
		case device_type_id: CameraSettings.DataSource = buferstr; break;
		case device_sn_id: CameraSettings.SNDevice = atoi(buferstr.c_str()); break;
		case inner_radius_id: CameraSettings.InnerRadius = atoi(buferstr.c_str()); break;
		case outer_radius_id: CameraSettings.OuterRadius = atoi(buferstr.c_str()); break;
		case resolution_acp_id: CameraSettings.ResolutionACP = atoi(buferstr.c_str()); break;
		case ev_acp_id: CameraSettings.EV_ACP = (float) atof(buferstr.c_str()); break;
		case pixel_size_id: CameraSettings.PixelSize = (float) atof(buferstr.c_str()); break;
		case focal_length_id: CameraSettings.FocalLength = (float) atof(buferstr.c_str()); break;
		case diametr_entrace_pupil_id: CameraSettings.DiameterEntrancePupil = (float) atof(buferstr.c_str()); break;
		case dark_level_id: CameraSettings.DarkLevel = (float) atof(buferstr.c_str()); break;
		case dark_sko_id: CameraSettings.DarkSKO = (float) atof(buferstr.c_str()); break;
		case limit_magn_id: CameraSettings.LimitingMagnitude = (float) atof(buferstr.c_str()); break;
		case stars_dist_id: CameraSettings.StarsDistribution = (float) atof(buferstr.c_str()); break;
		case function_distortion_id:
			{
				StringToFloatMass(buf, 2, buferstr);
				CameraSettings.DistortionFunction.PolynomialDegree = (unsigned short)buf[0];
				CameraSettings.DistortionFunction.CoefficientsCount = (unsigned short)buf[1];
				_FreeMemoryArray(CameraSettings.DistortionFunction.Coefficient);
				CameraSettings.DistortionFunction.Coefficient = new coefonaxis[CameraSettings.DistortionFunction.CoefficientsCount];
				for (int i = 0; i < CameraSettings.DistortionFunction.CoefficientsCount; i++)
				{
					fread(&buf_str[0], 1, 4, FStream); memmove(&sizedata, &buf_str[0], 4);
					memset(&Data_buf[0], 0, 10240); fread(&Data_buf[0], 1, sizedata, FStream);
					fread(&end_str[0], 1, 2, FStream);
					buferstr = (const char*) Data_buf;
					StringToFloatMass(buf, 2, buferstr);
					CameraSettings.DistortionFunction.Coefficient[i].X = buf[0];
					CameraSettings.DistortionFunction.Coefficient[i].Y = buf[1];
				}
				break;
			}
		case dead_pix_id:
			{
				StringToFloatMass(buf, 1, buferstr);
				CameraSettings.TableDeadObjects.RecordsCount = (unsigned short)buf[0];
				_FreeMemoryArray(CameraSettings.TableDeadObjects.PixelData);
				CameraSettings.TableDeadObjects.PixelData = new infopix[CameraSettings.TableDeadObjects.RecordsCount];
				for (int i = 0; i < CameraSettings.TableDeadObjects.RecordsCount; i++)
				{
					fread(&buf_str[0], 1, 4, FStream); memmove(&sizedata, &buf_str[0], 4);
					memset(&Data_buf[0], 0, 10240); fread(&Data_buf[0], 1, sizedata, FStream);
					fread(&end_str[0], 1, 2, FStream);
					buferstr = (const char*) Data_buf;
					StringToFloatMass(buf, 4, buferstr);
					CameraSettings.TableDeadObjects.PixelData[i].X_coordinate = (float) buf[0];
					CameraSettings.TableDeadObjects.PixelData[i].Y_coordinate = (float) buf[1];
					CameraSettings.TableDeadObjects.PixelData[i].BrightnessObj = (float) buf[2];
					CameraSettings.TableDeadObjects.PixelData[i].CoefficientObj = (unsigned short)buf[3];
				}
				break;
			}
		case matrixtemp_id: CameraSettings.MatrixTemp = (float) atof(buferstr.c_str()); break;
		case frequency_id: CameraSettings.Frequency = (unsigned short)atoi(buferstr.c_str()); break;
#pragma end_region
#pragma region FilterData
		case bottomright_id: FilterData.BottomRight = StringToBool(buferstr); break;
		case filter_pix_id:
			{
				pos_index = buferstr.find(" ");
				FilterData.FilteredPixelsCount = (unsigned short) atoi(buferstr.substr(0, pos_index).c_str());
				FilterData.TagFilter = buferstr.substr(pos_index + 1, buferstr.size() - pos_index);
				_FreeMemoryArray(FilterData.DataPixels);
				FilterData.DataPixels = new filterdata[FilterData.FilteredPixelsCount];
				for (int i = 0; i < FilterData.FilteredPixelsCount; i++)
				{
					fread(&buf_str[0], 1, 4, FStream); memmove(&sizedata, &buf_str[0], 4);
					memset(&Data_buf[0], 0, 10240); fread(&Data_buf[0], 1, sizedata, FStream);
					fread(&end_str[0], 1, 2, FStream);
					buferstr = (const char*) Data_buf;
					StringToFloatMass(buf, 3, buferstr);
					FilterData.DataPixels[i].X_coordinate = (unsigned short) buf[0];
					FilterData.DataPixels[i].Y_coordinate = (unsigned short) buf[1];
					FilterData.DataPixels[i].BrightnessPixel = (unsigned short) buf[2];
				}
				break;
			}
#pragma end_region
#pragma region starsinfo
		case rezstat_id: StarsData.RezStat = (unsigned short) strtol(buferstr.c_str(), NULL, 16); break;
		case epsilon_id: StarsData.Epsilon = (float) atof(buferstr.c_str()); break;
		case m_X_Y_Cyr_id:
			{
				StringToFloatMass(buf, 3, buferstr);
				StarsData.m_X = (float) buf[0];
				StarsData.m_Y = (float) buf[1];
				StarsData.m_Cur = (float) buf[2];
				break;
			}
		case local_recog_id: StarsData.LocalizedCount = atoi(buferstr.c_str()); break;
		case sizelocallist_id:
			{
				string str;
				StringToFloatMass(buf, 2, buferstr);
				StarsData.SizeLocalList = (int)buf[0];
				StarsData.RecognizedCount = (int)buf[1];
				_FreeMemoryArray(StarsData.StarsList);
				StarsData.StarsList = new starinfo[StarsData.SizeLocalList];
				memset(StarsData.StarsList, 0, sizeof(starinfo) * StarsData.SizeLocalList);
				for (int i = 0; i < StarsData.SizeLocalList; i++)
				{
					fread(&buf_str[0], 1, 4, FStream); memmove(&sizedata, &buf_str[0], 4);
					memset(&Data_buf[0], 0, 10240); fread(&Data_buf[0], 1, sizedata, FStream);
					for (unsigned int k = 0; k < sizedata; k++)
						if (Data_buf[k] == '\0')
							Data_buf[k] = '.';

					fread(&end_str[0], 1, 2, FStream);
					buferstr = (const char*) Data_buf;
					count_find_pos = 0;
					for (int j = 0; j < 9; j++)
					{
						if (buferstr.size() > 0)
						{
							pos_index = buferstr.find(" ");
							if (pos_index >= 0)
							{
								switch (count_find_pos)
								{
								case 0: StarsData.StarsList[i].BrightnessObject = (float) atof(buferstr.substr(0, pos_index).c_str()); break;
								case 1: StarsData.StarsList[i].PixelsCount = (float) atof(buferstr.substr(0, pos_index).c_str()); break;
								case 2: StarsData.StarsList[i].X_coordinate = (float) atof(buferstr.substr(0, pos_index).c_str()); break;
								case 3: StarsData.StarsList[i].Y_coordinate = (float) atof(buferstr.substr(0, pos_index).c_str()); break;
								case 4: StarsData.StarsList[i].NumberStar = (unsigned short) atoi(buferstr.substr(0, pos_index).c_str()); break;
								case 5: StarsData.StarsList[i].StellarMagnitude = (float) atof(buferstr.substr(0, pos_index).c_str()); break;
								case 6:
									{
										str = buferstr.substr(0, pos_index);
										memmove(&StarsData.StarsList[i].SpectralClass[0], str.c_str(), 2);
										break;
									}
								case 7: StarsData.StarsList[i].DX = (float) atof(buferstr.substr(0, pos_index).c_str()); break;

								}
								buferstr = buferstr.substr(pos_index + 1, buferstr.size() - pos_index);
								count_find_pos++;
							}
							else
								StarsData.StarsList[i].DY = (float) atof(buferstr.c_str());
						}
						else
							break;
					}
				}
				break;
			}
		case rec_id: StarsData.SimulatedFrame.strrec = (unsigned short)atoi(buferstr.c_str()); break;
		case sizestarlist_id:
			{
				StarsData.SimulatedFrame.SizeStarList = (unsigned short) atoi(buferstr.c_str());
				_FreeMemoryArray(StarsData.SimulatedFrame.StarRec);
				StarsData.SimulatedFrame.StarRec = new STARREC[StarsData.SimulatedFrame.SizeStarList];
				for (int i = 0; i < StarsData.SimulatedFrame.SizeStarList; i++)
				{
					fread(&buf_str[0], 1, 4, FStream); memmove(&sizedata, &buf_str[0], 4);
					memset(&Data_buf[0], 0, 10240); fread(&Data_buf[0], 1, sizedata, FStream);
					fread(&end_str[0], 1, 2, FStream);
					buferstr = (const char*) Data_buf;
					count_find_pos = 0;
					for (int j = 0; j < 6; j++)
					{
						pos_index = buferstr.find(" ");
						if (pos_index >= 0)
						{
							switch (count_find_pos)
							{
							case 0: StarsData.SimulatedFrame.StarRec[i].Ns = (unsigned short) atof(buferstr.substr(0, pos_index).c_str()); break;
							case 1: StarsData.SimulatedFrame.StarRec[i].Is = (unsigned long) atof(buferstr.substr(0, pos_index).c_str()); break;
							case 2: StarsData.SimulatedFrame.StarRec[i].Xs = (float) atof(buferstr.substr(0, pos_index).c_str()); break;
							case 3: StarsData.SimulatedFrame.StarRec[i].Ys = (float) atof(buferstr.substr(0, pos_index).c_str()); break;
							case 4: StarsData.SimulatedFrame.StarRec[i].Mv = (float) atof(buferstr.substr(0, pos_index).c_str()); break;
							}
							buferstr = buferstr.substr(pos_index + 1, buferstr.size() - pos_index);
							count_find_pos++;
						}
						else
							memmove(&StarsData.SimulatedFrame.StarRec[i].Sp[0], buferstr.c_str(), 2);
					}
				}
				break;
			}
		case rec_orient_angl_id: StringToFloatMass(StarsData.RecognizedOrientationAngles, 3, buferstr); break;
		case rec_orient_quat_id: StringToFloatMass(StarsData.RecognizedOrientationQuaternion, 4, buferstr); break;
		case rec_orient_matr_id: StringToFloatMass3D(StarsData.RecognizedOrientationMatrix, 3, 3, buferstr); break;
		case rec_angular_vel_id: StringToFloatMass(StarsData.RecognizedAngularVelocity, 3, buferstr); break;
#pragma end_region
#pragma region videoframe
		case exp_time_id: ImageData.ExposureTime = (float)atof(buferstr.c_str()); break;
		case stringframe_type_id: ImageData.FrameData.DataType = (unsigned int)atoi(buferstr.c_str()); break;
		case stringframe_deg_bin_id: ImageData.FrameData.DegreeBinning = (unsigned int)atoi(buferstr.c_str()); break;
		case stringframe_width_id: ImageData.FrameData.FrameWidth = (unsigned int)atoi(buferstr.c_str()); break;
		case stringframe_height_id: ImageData.FrameData.FrameHeight = (unsigned int)atoi(buferstr.c_str()); break;
		case stringframe_SizeData_id: ImageData.FrameData.SizeData = (unsigned int)atoi(buferstr.c_str()); break;

		case stringlines_count_id: ImageData.LinesData.LinesCount = (unsigned short)atoi(buferstr.c_str()); break;
		case stringlines_type_id: ImageData.LinesData.DataType = (unsigned int)atoi(buferstr.c_str()); break;
		case stringlines_deg_bin_id: ImageData.LinesData.DegreeBinning = (unsigned int)atoi(buferstr.c_str()); break;
		case stringlines_width_id: ImageData.LinesData.LinesWidth = (unsigned int)atoi(buferstr.c_str()); break;
		case stringlines_height_id: ImageData.LinesData.LinesHeight = (unsigned int)atoi(buferstr.c_str()); break;
		case stringlines_SizeData_id: ImageData.LinesData.SizeData = (unsigned int)atoi(buferstr.c_str()); break;
		case stringlines_info_id:
			{
				_FreeMemoryArray(ImageData.LinesData.Info);
				ImageData.LinesData.Info = new dataline[ImageData.LinesData.LinesCount];
				for (int i = 0; i < ImageData.LinesData.LinesCount; i++)
				{
					fread(&buf_str[0], 1, 4, FStream); memmove(&sizedata, &buf_str[0], 4);
					memset(&Data_buf[0], 0, 10240); fread(&Data_buf[0], 1, sizedata, FStream);
					fread(&end_str[0], 1, 2, FStream);
					buferstr = (const char*) Data_buf;
					StringToFloatMass(buf, 2, buferstr);
					ImageData.LinesData.Info[i].Y_FirstString = (int) buf[0];
					ImageData.LinesData.Info[i].CountString = (int) buf[1];
				}
				break;
			}
		case stringwindow_sizewindowlist_id: ImageData.WindowsData.SizeWindowList = (unsigned short)atoi(buferstr.c_str()); break;
		case stringwindow_count_id: ImageData.WindowsData.WindowCount = (unsigned short)atoi(buferstr.c_str()); break;
		case stringwindow_type_id: ImageData.WindowsData.DataType = (unsigned int)atoi(buferstr.c_str()); break;
		case stringwindow_deg_bin_id: ImageData.WindowsData.DegreeBinning = (unsigned int)atoi(buferstr.c_str()); break;
		case stringwindow_SizeData_id: ImageData.WindowsData.SizeData = (unsigned int)atoi(buferstr.c_str()); break;
		case stringwindow_flag_id: ImageData.WindowsData.BottomRight = StringToBool(buferstr); break;
		case stringwindow_info_id:
			{
				_FreeMemoryArray(ImageData.WindowsData.Info);
				ImageData.WindowsData.Info = new datawindow[ImageData.WindowsData.SizeWindowList];
				for (int i = 0; i < ImageData.WindowsData.SizeWindowList; i++)
				{
					fread(&buf_str[0], 1, 4, FStream); memmove(&sizedata, &buf_str[0], 4);
					memset(&Data_buf[0], 0, 10240); fread(&Data_buf[0], 1, sizedata, FStream);
					fread(&end_str[0], 1, 2, FStream);
					buferstr = (const char*) Data_buf;
					StringToFloatMass(buf, 10, buferstr);
					ImageData.WindowsData.Info[i].WindowWidth = (unsigned int) buf[0];
					ImageData.WindowsData.Info[i].WindowHeight = (unsigned int) buf[1];
					ImageData.WindowsData.Info[i].X = (unsigned int) buf[2];
					ImageData.WindowsData.Info[i].Y = (unsigned int) buf[3];
					ImageData.WindowsData.Info[i].Limit = (unsigned int) buf[4];
					ImageData.WindowsData.Info[i].ObjCount = (unsigned int) buf[5];
					ImageData.WindowsData.Info[i].SKO = (float) buf[6];
					ImageData.WindowsData.Info[i].Average = (float) buf[7];
					ImageData.WindowsData.Info[i].ZipX = (unsigned short) buf[8];
					ImageData.WindowsData.Info[i].ZipY = (unsigned short) buf[9];
				}
				break;
			}
		case stringframe_id:
			{
				StringToFloatMass(buf, 5, buferstr);
				ImageData.ExposureTime = (float) buf[0];
				ImageData.FrameData.DataType = (unsigned int) buf[1];
				ImageData.FrameData.DegreeBinning = (unsigned int) buf[2];
				ImageData.FrameData.FrameWidth = (unsigned int) buf[3];
				ImageData.FrameData.FrameHeight = (unsigned int) buf[4];
				break;
			}
		case stringlines_id:
			{
				StringToFloatMass(buf, 6, buferstr);
				ImageData.ExposureTime = (float) buf[0];
				ImageData.LinesData.LinesCount = (unsigned short) buf[1];
				ImageData.LinesData.DataType = (unsigned int) buf[2];
				ImageData.LinesData.DegreeBinning = (unsigned int) buf[3];
				ImageData.LinesData.LinesWidth = (unsigned int) buf[4];
				ImageData.LinesData.LinesHeight = (unsigned int) buf[5];
				_FreeMemoryArray(ImageData.LinesData.Info);
				ImageData.LinesData.Info = new dataline[ImageData.LinesData.LinesCount];
				for (int i = 0; i < ImageData.LinesData.LinesCount; i++)
				{
					fread(&buf_str[0], 1, 4, FStream); memmove(&sizedata, &buf_str[0], 4);
					memset(&Data_buf[0], 0, 10240); fread(&Data_buf[0], 1, sizedata, FStream);
					fread(&end_str[0], 1, 2, FStream);
					buferstr = (const char*) Data_buf;
					StringToFloatMass(buf, 2, buferstr);
					ImageData.LinesData.Info[i].Y_FirstString = (int) buf[0];
					ImageData.LinesData.Info[i].CountString = (int) buf[1];
				}
				break;
			}
		case stringwindow_id:
			{
				StringToFloatMass(buf, 5, buferstr);
				ImageData.ExposureTime = (float) buf[0];
				ImageData.WindowsData.SizeWindowList = (unsigned short) buf[1];
				ImageData.WindowsData.DataType = (unsigned int) buf[2];
				ImageData.WindowsData.DegreeBinning = (unsigned int) buf[3];
				ImageData.WindowsData.SizeData = (unsigned int) buf[4];
				_FreeMemoryArray(ImageData.WindowsData.Info);
				ImageData.WindowsData.Info = new datawindow[ImageData.WindowsData.SizeWindowList];
				for (int i = 0; i < ImageData.WindowsData.SizeWindowList; i++)
				{
					fread(&buf_str[0], 1, 4, FStream); memmove(&sizedata, &buf_str[0], 4);
					memset(&Data_buf[0], 0, 10240); fread(&Data_buf[0], 1, sizedata, FStream);
					fread(&end_str[0], 1, 2, FStream);
					buferstr = (const char*) Data_buf;
					StringToFloatMass(buf, 8, buferstr);
					ImageData.WindowsData.Info[i].WindowWidth = (unsigned int) buf[0];
					ImageData.WindowsData.Info[i].WindowHeight = (unsigned int) buf[1];
					ImageData.WindowsData.Info[i].X = (unsigned int) buf[2];
					ImageData.WindowsData.Info[i].Y = (unsigned int) buf[3];
					ImageData.WindowsData.Info[i].Limit = (unsigned int) buf[4];
					ImageData.WindowsData.Info[i].ObjCount = (unsigned int) buf[5];
					ImageData.WindowsData.Info[i].SKO = (float) buf[6];
					ImageData.WindowsData.Info[i].Average = (float) buf[7];
				}
				break;
			}
		case videoframe_id:
			{
				if (ImageData.FrameData.SizeData != 0)
				{
					_FreeMemoryArray(ImageData.FrameData.Data);
					switch (ImageData.FrameData.DataType)
					{
					case 0: ImageData.FrameData.Data = (void*)new unsigned char[ImageData.FrameData.SizeData]; break;
					case 1: ImageData.FrameData.Data = (void*)new unsigned short[ImageData.FrameData.SizeData]; break;
					case 2: ImageData.FrameData.Data = (void*)new float[ImageData.FrameData.SizeData]; break;
					}
					fread(ImageData.FrameData.Data, MassType[ImageData.FrameData.DataType], ImageData.FrameData.SizeData, FStream);

				}
				fread(&end_str[0], 1, 2, FStream);
				break;
			}
		case videolines_id:
			{
				if (ImageData.LinesData.SizeData != 0)
				{
					_FreeMemoryArray(ImageData.LinesData.Data);
					switch (ImageData.LinesData.DataType)
					{
					case 0: ImageData.LinesData.Data = (void*)new unsigned char[ImageData.LinesData.SizeData]; break;
					case 1: ImageData.LinesData.Data = (void*)new unsigned short[ImageData.LinesData.SizeData]; break;
					case 2: ImageData.LinesData.Data = (void*)new float[ImageData.LinesData.SizeData]; break;
					}
					fread(ImageData.LinesData.Data, MassType[ImageData.LinesData.DataType], ImageData.LinesData.SizeData, FStream);

				}
				fread(&end_str[0], 1, 2, FStream);
				break;
			}
		case videowindow_id:
			{
				if (ImageData.WindowsData.SizeData != 0)
				{
					_FreeMemoryArray(ImageData.WindowsData.Data);
					switch (ImageData.WindowsData.DataType)
					{
					case 0: ImageData.WindowsData.Data = (void*)new unsigned char[ImageData.WindowsData.SizeData]; break;
					case 1: ImageData.WindowsData.Data = (void*)new unsigned short[ImageData.WindowsData.SizeData]; break;
					case 2: ImageData.WindowsData.Data = (void*)new float[ImageData.WindowsData.SizeData]; break;
					}
					fread(ImageData.WindowsData.Data, MassType[ImageData.WindowsData.DataType], ImageData.WindowsData.SizeData, FStream);

				}
				fread(&end_str[0], 1, 2, FStream);
				break;
			}
#pragma end_region
		}
	}
	_FreeMemoryArray(end_str);
	_FreeMemoryArray(bufer);
	_FreeMemoryArray(Data_buf);
	fclose(FStream);
	return true;
}
// ---------------------------------------------------------------------------

unsigned char* IKI_img::ZipStructure(unsigned int &SizeInfo)
{
	unsigned int position = 0;
	unsigned int size_id; // размер пакета, без размера признака

	unsigned int whenwhere_size =
		sizeof(Georeferencing.DateTime)+
		sizeof(size_id) + // размер последующего текста,
		Georeferencing.SoftwareVersion.size()+ // сам текст
		sizeof(Georeferencing.FrameNumber)+
		sizeof(Georeferencing.ProtonsDensity)+
		sizeof(Georeferencing.TracksBrightness)+
		sizeof(Georeferencing.MoonVelocity)+
		sizeof(Georeferencing.MoonBrightness)+
		sizeof(Georeferencing.BrightnessLeft)+
		sizeof(Georeferencing.BrightnessRight)+
		sizeof(Georeferencing.Era)+
		sizeof(Georeferencing.OrbitEccentricity)+
		sizeof(Georeferencing.LengthSemiaxis)+
		sizeof(Georeferencing.LengthRadiusVector)+
		sizeof(Georeferencing.LongitudeAscendingNode)+
		sizeof(Georeferencing.InclinationOrbit)+
		sizeof(Georeferencing.ArgumentPerigee)+
		sizeof(Georeferencing.ArgumentLatitude)+
		sizeof(Georeferencing.DeviceAngularVelocity[3])+
		sizeof(Georeferencing.DeviceAngularAcceleration[3])+
		sizeof(Georeferencing.ProjectionLinearVelocity[3])+
		sizeof(Georeferencing.OrientationAngles[3])+
		sizeof(Georeferencing.OrientationQuaternion[4])+
		sizeof(Georeferencing.OrientationMatrix[3][3])+
		sizeof(Georeferencing.YearAberrationApply)+
		sizeof(Georeferencing.SatelliteAberrationApply)+
		sizeof(Georeferencing.PrecessionNutationApply)+
		sizeof(Georeferencing.PhotonNoiseApply)+
		sizeof(Georeferencing.DistortionApply);

	unsigned int devicetype_size =
		sizeof(size_id) +
		CameraSettings.DataSource.size()+
		sizeof(CameraSettings.SNDevice)+
		sizeof(CameraSettings.InnerRadius)+
		sizeof(CameraSettings.OuterRadius)+
		sizeof(CameraSettings.ResolutionACP)+
		sizeof(CameraSettings.EV_ACP)+
		sizeof(CameraSettings.PixelSize)+
		sizeof(CameraSettings.FocalLength)+
		sizeof(CameraSettings.DiameterEntrancePupil)+
		sizeof(CameraSettings.DarkLevel)+
		sizeof(CameraSettings.DarkSKO)+
		sizeof(CameraSettings.LimitingMagnitude)+
		sizeof(CameraSettings.StarsDistribution)+
		sizeof(CameraSettings.DistortionFunction.PolynomialDegree)+
		sizeof(CameraSettings.DistortionFunction.CoefficientsCount)+
		sizeof(coefonaxis) * CameraSettings.DistortionFunction.CoefficientsCount +
		sizeof(CameraSettings.TableDeadObjects.RecordsCount)+
		sizeof(infopix) * CameraSettings.TableDeadObjects.RecordsCount;

	unsigned int filterframe_size =
		sizeof(size_id) +
		FilterData.TagFilter.size()+
		sizeof(FilterData.FilteredPixelsCount)+
		sizeof(FilterData.BottomRight)+
		sizeof(filterdata) * FilterData.FilteredPixelsCount;

	unsigned int starsinfo_size =
		sizeof(StarsData.RezStat)+
		sizeof(StarsData.Epsilon)+
		sizeof(StarsData.m_X)+sizeof(StarsData.m_Y)+sizeof(StarsData.m_Cur)+
		sizeof(StarsData.LocalizedCount)+
		sizeof(StarsData.RecognizedCount)+
		sizeof(starinfo) * StarsData.LocalizedCount +
		sizeof(StarsData.SimulatedFrame.strrec)+
		sizeof(STARREC) * StarsData.SimulatedFrame.strrec +
		sizeof(StarsData.RecognizedOrientationAngles)+
		sizeof(StarsData.RecognizedOrientationQuaternion)+
		sizeof(StarsData.RecognizedOrientationMatrix);

	unsigned int videodata_size =
		sizeof(ImageData.ExposureTime)+
		sizeof(ImageData.FrameData.DataType)+
		sizeof(ImageData.FrameData.DegreeBinning)+
		sizeof(ImageData.FrameData.FrameWidth)+
		sizeof(ImageData.FrameData.FrameHeight) +
		(MassType[ImageData.FrameData.DataType] * ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight)+

		sizeof(ImageData.LinesData.LinesCount)+
		sizeof(ImageData.LinesData.DataType)+
		sizeof(ImageData.LinesData.DegreeBinning)+
		sizeof(ImageData.LinesData.LinesWidth)+
		sizeof(ImageData.LinesData.LinesHeight)+
		sizeof(dataline) * ImageData.LinesData.LinesCount +
		(MassType[ImageData.LinesData.DataType] * ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight)+

		sizeof(ImageData.WindowsData.WindowCount)+
		sizeof(ImageData.WindowsData.DataType)+
		sizeof(ImageData.WindowsData.DegreeBinning)+
		sizeof(ImageData.WindowsData.SizeData)+
		sizeof(ImageData.WindowsData.BottomRight) +
		(sizeof(datawindow) * ImageData.WindowsData.WindowCount) +
		(MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData);

	SizeInfo = whenwhere_size + devicetype_size + filterframe_size + starsinfo_size + videodata_size;
	unsigned char* Array = new unsigned char[SizeInfo];
	memset(Array, 0, SizeInfo);

	// Move(&Georeferencing.DateTime, &Array[position], sizeof(Georeferencing.DateTime));
	memmove(&Array[position], &Georeferencing.DateTime, sizeof(Georeferencing.DateTime)); position += sizeof(Georeferencing.DateTime);
	size_id = Georeferencing.SoftwareVersion.size();
	// Move(&size_id, &Array[position], sizeof(size_id));
	memmove(&Array[position], &size_id, sizeof(size_id)); position += sizeof(size_id); // запись длины текста
	// Move(Georeferencing.SoftwareVersion.c_str(), &Array[position], size_id);
	memmove(&Array[position], Georeferencing.SoftwareVersion.c_str(), size_id); position += size_id;
	// Move(&Georeferencing.FrameNumber, &Array[position], sizeof(Georeferencing.FrameNumber));
	memmove(&Array[position], &Georeferencing.FrameNumber, sizeof(Georeferencing.FrameNumber)); position += sizeof(Georeferencing.FrameNumber);
	// Move(&Georeferencing.ProtonsDensity, &Array[position], sizeof(Georeferencing.ProtonsDensity));
	memmove(&Array[position], &Georeferencing.ProtonsDensity, sizeof(Georeferencing.ProtonsDensity)); position += sizeof(Georeferencing.ProtonsDensity);
	// Move(&Georeferencing.TracksBrightness, &Array[position], sizeof(Georeferencing.TracksBrightness));
	memmove(&Array[position], &Georeferencing.TracksBrightness, sizeof(Georeferencing.TracksBrightness)); position += sizeof(Georeferencing.TracksBrightness);
	// Move(&Georeferencing.MoonVelocity, &Array[position], sizeof(Georeferencing.MoonVelocity));
	memmove(&Array[position], &Georeferencing.MoonVelocity, sizeof(Georeferencing.MoonVelocity)); position += sizeof(Georeferencing.MoonVelocity);
	// Move(&Georeferencing.MoonBrightness, &Array[position], sizeof(Georeferencing.MoonBrightness));
	memmove(&Array[position], &Georeferencing.MoonBrightness, sizeof(Georeferencing.MoonBrightness)); position += sizeof(Georeferencing.MoonBrightness);
	// Move(&Georeferencing.BrightnessLeft, &Array[position], sizeof(Georeferencing.BrightnessLeft));
	memmove(&Array[position], &Georeferencing.BrightnessLeft, sizeof(Georeferencing.BrightnessLeft)); position += sizeof(Georeferencing.BrightnessLeft);
	// Move(&Georeferencing.BrightnessRight, &Array[position], sizeof(Georeferencing.BrightnessRight));
	memmove(&Array[position], &Georeferencing.BrightnessRight, sizeof(Georeferencing.BrightnessRight)); position += sizeof(Georeferencing.BrightnessRight);
	// Move(&Georeferencing.Era, &Array[position], sizeof(Georeferencing.Era));
	memmove(&Array[position], &Georeferencing.Era, sizeof(Georeferencing.Era)); position += sizeof(Georeferencing.Era);
	// Move(&Georeferencing.OrbitEccentricity, &Array[position], sizeof(Georeferencing.OrbitEccentricity));
	memmove(&Array[position], &Georeferencing.OrbitEccentricity, sizeof(Georeferencing.OrbitEccentricity)); position += sizeof(Georeferencing.OrbitEccentricity);
	// Move(&Georeferencing.LengthSemiaxis, &Array[position], sizeof(Georeferencing.LengthSemiaxis));
	memmove(&Array[position], &Georeferencing.LengthSemiaxis, sizeof(Georeferencing.LengthSemiaxis)); position += sizeof(Georeferencing.LengthSemiaxis);
	// Move(&Georeferencing.LengthRadiusVector, &Array[position], sizeof(Georeferencing.LengthRadiusVector));
	memmove(&Array[position], &Georeferencing.LengthRadiusVector, sizeof(Georeferencing.LengthRadiusVector)); position += sizeof(Georeferencing.LengthRadiusVector);
	// Move(&Georeferencing.LongitudeAscendingNode, &Array[position], sizeof(Georeferencing.LongitudeAscendingNode));
	memmove(&Array[position], &Georeferencing.LongitudeAscendingNode, sizeof(Georeferencing.LongitudeAscendingNode)); position += sizeof(Georeferencing.LongitudeAscendingNode);
	// Move(&Georeferencing.InclinationOrbit, &Array[position], sizeof(Georeferencing.InclinationOrbit));
	memmove(&Array[position], &Georeferencing.InclinationOrbit, sizeof(Georeferencing.InclinationOrbit)); position += sizeof(Georeferencing.InclinationOrbit);
	// Move(&Georeferencing.ArgumentPerigee, &Array[position], sizeof(Georeferencing.ArgumentPerigee));
	memmove(&Array[position], &Georeferencing.ArgumentPerigee, sizeof(Georeferencing.ArgumentPerigee)); position += sizeof(Georeferencing.ArgumentPerigee);
	// Move(&Georeferencing.ArgumentLatitude, &Array[position], sizeof(Georeferencing.ArgumentLatitude));
	memmove(&Array[position], &Georeferencing.ArgumentLatitude, sizeof(Georeferencing.ArgumentLatitude)); position += sizeof(Georeferencing.ArgumentLatitude);
	// Move(&Georeferencing.DeviceAngularVelocity, &Array[position], sizeof(Georeferencing.DeviceAngularVelocity));
	memmove(&Array[position], &Georeferencing.DeviceAngularVelocity, sizeof(Georeferencing.DeviceAngularVelocity)); position += sizeof(Georeferencing.DeviceAngularVelocity);
	// Move(&Georeferencing.DeviceAngularAcceleration, &Array[position], sizeof(Georeferencing.DeviceAngularAcceleration));
	memmove(&Array[position], &Georeferencing.DeviceAngularAcceleration, sizeof(Georeferencing.DeviceAngularAcceleration)); position += sizeof(Georeferencing.DeviceAngularAcceleration);
	// Move(&Georeferencing.ProjectionLinearVelocity, &Array[position], sizeof(Georeferencing.ProjectionLinearVelocity));
	memmove(&Array[position], &Georeferencing.ProjectionLinearVelocity, sizeof(Georeferencing.ProjectionLinearVelocity)); position += sizeof(Georeferencing.ProjectionLinearVelocity);
	// Move(&Georeferencing.OrientationAngles, &Array[position], sizeof(Georeferencing.OrientationAngles));
	memmove(&Array[position], &Georeferencing.OrientationAngles, sizeof(Georeferencing.OrientationAngles)); position += sizeof(Georeferencing.OrientationAngles);
	// Move(&Georeferencing.OrientationQuaternion, &Array[position], sizeof(Georeferencing.OrientationQuaternion));
	memmove(&Array[position], &Georeferencing.OrientationQuaternion, sizeof(Georeferencing.OrientationQuaternion)); position += sizeof(Georeferencing.OrientationQuaternion);
	// Move(&Georeferencing.OrientationMatrix, &Array[position], sizeof(Georeferencing.OrientationMatrix));
	memmove(&Array[position], &Georeferencing.OrientationMatrix, sizeof(Georeferencing.OrientationMatrix)); position += sizeof(Georeferencing.OrientationMatrix);
	// Move(&Georeferencing.YearAberrationApply, &Array[position], sizeof(Georeferencing.YearAberrationApply));
	memmove(&Array[position], &Georeferencing.YearAberrationApply, sizeof(Georeferencing.YearAberrationApply)); position += sizeof(Georeferencing.YearAberrationApply);
	// Move(&Georeferencing.SatelliteAberrationApply, &Array[position], sizeof(Georeferencing.SatelliteAberrationApply));
	memmove(&Array[position], &Georeferencing.SatelliteAberrationApply, sizeof(Georeferencing.SatelliteAberrationApply)); position += sizeof(Georeferencing.SatelliteAberrationApply);
	// Move(&Georeferencing.PrecessionNutationApply, &Array[position], sizeof(Georeferencing.PrecessionNutationApply));
	memmove(&Array[position], &Georeferencing.PrecessionNutationApply, sizeof(Georeferencing.PrecessionNutationApply)); position += sizeof(Georeferencing.PrecessionNutationApply);
	// Move(&Georeferencing.PhotonNoiseApply, &Array[position], sizeof(Georeferencing.PhotonNoiseApply));
	memmove(&Array[position], &Georeferencing.PhotonNoiseApply, sizeof(Georeferencing.PhotonNoiseApply)); position += sizeof(Georeferencing.PhotonNoiseApply);
	// Move(&Georeferencing.DistortionApply, &Array[position], sizeof(Georeferencing.DistortionApply));
	memmove(&Array[position], &Georeferencing.DistortionApply, sizeof(Georeferencing.DistortionApply)); position += sizeof(Georeferencing.DistortionApply);

	size_id = CameraSettings.DataSource.size();
	// Move(&size_id, &Array[position], sizeof(size_id));
	memmove(&Array[position], &size_id, sizeof(size_id)); position += sizeof(size_id); // запись длины текста
	// Move(CameraSettings.DataSource.c_str(), &Array[position], size_id);
	memmove(&Array[position], CameraSettings.DataSource.c_str(), size_id); position += size_id;
	// Move(&CameraSettings.SNDevice, &Array[position], sizeof(CameraSettings.SNDevice));
	memmove(&Array[position], &CameraSettings.SNDevice, sizeof(CameraSettings.SNDevice)); position += sizeof(CameraSettings.SNDevice);
	// Move(&CameraSettings.InnerRadius, &Array[position], sizeof(CameraSettings.InnerRadius));
	memmove(&Array[position], &CameraSettings.InnerRadius, sizeof(CameraSettings.InnerRadius)); position += sizeof(CameraSettings.InnerRadius);
	// Move(&CameraSettings.OuterRadius, &Array[position], sizeof(CameraSettings.OuterRadius));
	memmove(&Array[position], &CameraSettings.OuterRadius, sizeof(CameraSettings.OuterRadius)); position += sizeof(CameraSettings.OuterRadius);
	// Move(&CameraSettings.ResolutionACP, &Array[position], sizeof(CameraSettings.ResolutionACP));
	memmove(&Array[position], &CameraSettings.ResolutionACP, sizeof(CameraSettings.ResolutionACP)); position += sizeof(CameraSettings.ResolutionACP);
	// Move(&CameraSettings.EV_ACP, &Array[position], sizeof(CameraSettings.EV_ACP));
	memmove(&Array[position], &CameraSettings.EV_ACP, sizeof(CameraSettings.EV_ACP)); position += sizeof(CameraSettings.EV_ACP);
	// Move(&CameraSettings.PixelSize, &Array[position], sizeof(CameraSettings.PixelSize));
	memmove(&Array[position], &CameraSettings.PixelSize, sizeof(CameraSettings.PixelSize)); position += sizeof(CameraSettings.PixelSize);
	// Move(&CameraSettings.FocalLength, &Array[position], sizeof(CameraSettings.FocalLength));
	memmove(&Array[position], &CameraSettings.FocalLength, sizeof(CameraSettings.FocalLength)); position += sizeof(CameraSettings.FocalLength);
	// Move(&CameraSettings.DiameterEntrancePupil, &Array[position], sizeof(CameraSettings.DiameterEntrancePupil));
	memmove(&Array[position], &CameraSettings.DiameterEntrancePupil, sizeof(CameraSettings.DiameterEntrancePupil)); position += sizeof(CameraSettings.DiameterEntrancePupil);
	// Move(&CameraSettings.DarkLevel, &Array[position], sizeof(CameraSettings.DarkLevel));
	memmove(&Array[position], &CameraSettings.DarkLevel, sizeof(CameraSettings.DarkLevel)); position += sizeof(CameraSettings.DarkLevel);
	// Move(&CameraSettings.DarkSKO, &Array[position], sizeof(CameraSettings.DarkSKO));
	memmove(&Array[position], &CameraSettings.DarkSKO, sizeof(CameraSettings.DarkSKO)); position += sizeof(CameraSettings.DarkSKO);
	// Move(&CameraSettings.LimitingMagnitude, &Array[position], sizeof(CameraSettings.LimitingMagnitude));
	memmove(&Array[position], &CameraSettings.LimitingMagnitude, sizeof(CameraSettings.LimitingMagnitude)); position += sizeof(CameraSettings.LimitingMagnitude);
	// Move(&CameraSettings.StarsDistribution, &Array[position], sizeof(CameraSettings.StarsDistribution));
	memmove(&Array[position], &CameraSettings.StarsDistribution, sizeof(CameraSettings.StarsDistribution)); position += sizeof(CameraSettings.StarsDistribution);
	// Move(&CameraSettings.DistortionFunction.PolynomialDegree, &Array[position], sizeof(CameraSettings.DistortionFunction.PolynomialDegree));
	memmove(&Array[position], &CameraSettings.DistortionFunction.PolynomialDegree, sizeof(CameraSettings.DistortionFunction.PolynomialDegree)); position += sizeof(CameraSettings.DistortionFunction.PolynomialDegree);
	// Move(&CameraSettings.DistortionFunction.CoefficientsCount, &Array[position], sizeof(CameraSettings.DistortionFunction.CoefficientsCount));
	memmove(&Array[position], &CameraSettings.DistortionFunction.CoefficientsCount, sizeof(CameraSettings.DistortionFunction.CoefficientsCount)); position += sizeof(CameraSettings.DistortionFunction.CoefficientsCount);
	for (int i = 0; i < CameraSettings.DistortionFunction.CoefficientsCount; i++)
	{
		// Move(&CameraSettings.DistortionFunction.Coefficient[i], &Array[position], sizeof(coefonaxis));
		memmove(&Array[position], &CameraSettings.DistortionFunction.Coefficient[i], sizeof(coefonaxis)); position += sizeof(coefonaxis);
	}
	// Move(&CameraSettings.TableDeadObjects.RecordsCount, &Array[position], sizeof(CameraSettings.TableDeadObjects.RecordsCount));
	memmove(&Array[position], &CameraSettings.TableDeadObjects.RecordsCount, sizeof(CameraSettings.TableDeadObjects.RecordsCount)); position += sizeof(CameraSettings.TableDeadObjects.RecordsCount);
	for (int i = 0; i < CameraSettings.TableDeadObjects.RecordsCount; i++)
	{
		// Move(&CameraSettings.TableDeadObjects.PixelData[i], &Array[position], sizeof(infopix));
		memmove(&Array[position], &CameraSettings.TableDeadObjects.PixelData[i], sizeof(infopix)); position += sizeof(infopix);
	}

	size_id = FilterData.TagFilter.size();
	// Move(&size_id, &Array[position], sizeof(size_id));
	memmove(&Array[position], &size_id, sizeof(size_id)); position += sizeof(size_id); // запись длины текста
	// Move(FilterData.TagFilter.c_str(), &Array[position], size_id);
	memmove(&Array[position], FilterData.TagFilter.c_str(), size_id); position += size_id;
	// Move(&FilterData.FilteredPixelsCount, &Array[position], sizeof(FilterData.FilteredPixelsCount));
	memmove(&Array[position], &FilterData.FilteredPixelsCount, sizeof(FilterData.FilteredPixelsCount)); position += sizeof(FilterData.FilteredPixelsCount);
	// Move(&FilterData.BottomRight, &Array[position], sizeof(FilterData.BottomRight));
	memmove(&Array[position], &FilterData.BottomRight, sizeof(FilterData.BottomRight)); position += sizeof(FilterData.BottomRight);
	for (int i = 0; i < FilterData.FilteredPixelsCount; i++)
	{
		// Move(&FilterData.DataPixels[i], &Array[position], sizeof(filterdata));
		memmove(&Array[position], &FilterData.DataPixels[i], sizeof(filterdata)); position += sizeof(filterdata);
	}

	// Move(&StarsData.RezStat, &Array[position], sizeof(StarsData.RezStat));
	memmove(&Array[position], &StarsData.RezStat, sizeof(StarsData.RezStat)); position += sizeof(StarsData.RezStat);
	// Move(&StarsData.Epsilon, &Array[position], sizeof(StarsData.Epsilon));
	memmove(&Array[position], &StarsData.Epsilon, sizeof(StarsData.Epsilon)); position += sizeof(StarsData.Epsilon);
	// Move(&StarsData.m_X, &Array[position], sizeof(StarsData.m_X));
	memmove(&Array[position], &StarsData.m_X, sizeof(StarsData.m_X)); position += sizeof(StarsData.m_X);
	// Move(&StarsData.m_Y, &Array[position], sizeof(StarsData.m_Y));
	memmove(&Array[position], &StarsData.m_Y, sizeof(StarsData.m_Y)); position += sizeof(StarsData.m_Y);
	// Move(&StarsData.m_Cur, &Array[position], sizeof(StarsData.m_Cur));
	memmove(&Array[position], &StarsData.m_Cur, sizeof(StarsData.m_Cur)); position += sizeof(StarsData.m_Cur);
	// Move(&StarsData.LocalizedCount, &Array[position], sizeof(StarsData.LocalizedCount));
	memmove(&Array[position], &StarsData.LocalizedCount, sizeof(StarsData.LocalizedCount)); position += sizeof(StarsData.LocalizedCount);
	// Move(&StarsData.RecognizedCount, &Array[position], sizeof(StarsData.RecognizedCount));
	memmove(&Array[position], &StarsData.RecognizedCount, sizeof(StarsData.RecognizedCount)); position += sizeof(StarsData.RecognizedCount);
	for (int i = 0; i < StarsData.LocalizedCount; i++)
	{
		// Move(&StarsData.StarsList[i], &Array[position], sizeof(starinfo));
		memmove(&Array[position], &StarsData.StarsList[i], sizeof(starinfo)); position += sizeof(starinfo);
	}
	// Move(&StarsData.SimulatedFrame.strrec, &Array[position], sizeof(StarsData.SimulatedFrame.strrec));
	memmove(&Array[position], &StarsData.SimulatedFrame.strrec, sizeof(StarsData.SimulatedFrame.strrec)); position += sizeof(StarsData.SimulatedFrame.strrec);
	for (int i = 0; i < StarsData.SimulatedFrame.strrec; i++)
	{
		// Move(&StarsData.SimulatedFrame.StarRec[i], &Array[position], sizeof(starinfo));
		memmove(&Array[position], &StarsData.SimulatedFrame.StarRec[i], sizeof(starinfo)); position += sizeof(STARREC);
	}
	// Move(&StarsData.RecognizedOrientationAngles, &Array[position], sizeof(StarsData.RecognizedOrientationAngles));
	memmove(&Array[position], &StarsData.RecognizedOrientationAngles, sizeof(StarsData.RecognizedOrientationAngles)); position += sizeof(StarsData.RecognizedOrientationAngles);
	// Move(&StarsData.RecognizedOrientationQuaternion, &Array[position], sizeof(StarsData.RecognizedOrientationQuaternion));
	memmove(&Array[position], &StarsData.RecognizedOrientationQuaternion, sizeof(StarsData.RecognizedOrientationQuaternion)); position += sizeof(StarsData.RecognizedOrientationQuaternion);
	// Move(&StarsData.RecognizedOrientationMatrix, &Array[position], sizeof(StarsData.RecognizedOrientationMatrix));
	memmove(&Array[position], &StarsData.RecognizedOrientationMatrix, sizeof(StarsData.RecognizedOrientationMatrix)); position += sizeof(StarsData.RecognizedOrientationMatrix);

	// Move(&ImageData.ExposureTime, &Array[position], sizeof(ImageData.ExposureTime));
	memmove(&Array[position], &ImageData.ExposureTime, sizeof(ImageData.ExposureTime)); position += sizeof(ImageData.ExposureTime);
	// Move(&ImageData.FrameData.DataType, &Array[position], sizeof(ImageData.FrameData.DataType));
	memmove(&Array[position], &ImageData.FrameData.DataType, sizeof(ImageData.FrameData.DataType)); position += sizeof(ImageData.FrameData.DataType);
	// Move(&ImageData.FrameData.DegreeBinning, &Array[position], sizeof(ImageData.FrameData.DegreeBinning));
	memmove(&Array[position], &ImageData.FrameData.DegreeBinning, sizeof(ImageData.FrameData.DegreeBinning)); position += sizeof(ImageData.FrameData.DegreeBinning);
	// Move(&ImageData.FrameData.FrameWidth, &Array[position], sizeof(ImageData.FrameData.FrameWidth));
	memmove(&Array[position], &ImageData.FrameData.FrameWidth, sizeof(ImageData.FrameData.FrameWidth)); position += sizeof(ImageData.FrameData.FrameWidth);
	// Move(&ImageData.FrameData.FrameHeight, &Array[position], sizeof(ImageData.FrameData.FrameHeight));
	memmove(&Array[position], &ImageData.FrameData.FrameHeight, sizeof(ImageData.FrameData.FrameHeight)); position += sizeof(ImageData.FrameData.FrameHeight);
	// Move(ImageData.FrameData.Data, &Array[position], (MassType[ImageData.FrameData.DataType] * ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight));
	memmove(&Array[position], &ImageData.FrameData.Data, (MassType[ImageData.FrameData.DataType] * ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight)); position += (MassType[ImageData.FrameData.DataType] * ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight);

	// Move(&ImageData.LinesData.LinesCount, &Array[position], sizeof(ImageData.LinesData.LinesCount));
	memmove(&Array[position], &ImageData.LinesData.LinesCount, sizeof(ImageData.LinesData.LinesCount)); position += sizeof(ImageData.LinesData.LinesCount);
	// Move(&ImageData.LinesData.DataType, &Array[position], sizeof(ImageData.LinesData.DataType));
	memmove(&Array[position], &ImageData.LinesData.DataType, sizeof(ImageData.LinesData.DataType)); position += sizeof(ImageData.LinesData.DataType);
	// Move(&ImageData.LinesData.DegreeBinning, &Array[position], sizeof(ImageData.LinesData.DegreeBinning));
	memmove(&Array[position], &ImageData.LinesData.DegreeBinning, sizeof(ImageData.LinesData.DegreeBinning)); position += sizeof(ImageData.LinesData.DegreeBinning);
	// Move(&ImageData.LinesData.LinesWidth, &Array[position], sizeof(ImageData.LinesData.LinesWidth));
	memmove(&Array[position], &ImageData.LinesData.LinesWidth, sizeof(ImageData.LinesData.LinesWidth)); position += sizeof(ImageData.LinesData.LinesWidth);
	// Move(&ImageData.LinesData.LinesHeight, &Array[position], sizeof(ImageData.LinesData.LinesHeight));
	memmove(&Array[position], &ImageData.LinesData.LinesHeight, sizeof(ImageData.LinesData.LinesHeight)); position += sizeof(ImageData.LinesData.LinesHeight);
	for (int i = 0; i < ImageData.LinesData.LinesCount; i++)
	{
		// Move(&ImageData.LinesData.Info[i], &Array[position], sizeof(dataline));
		memmove(&Array[position], &ImageData.LinesData.Info[i], sizeof(dataline)); position += sizeof(dataline);
	}
	// Move(ImageData.LinesData.Data, &Array[position], (MassType[ImageData.LinesData.DataType] * ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight));
	memmove(&Array[position], ImageData.LinesData.Data, (MassType[ImageData.LinesData.DataType] * ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight)); position += (MassType[ImageData.LinesData.DataType] * ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight);

	// Move(&ImageData.WindowsData.WindowCount, &Array[position], sizeof(ImageData.WindowsData.WindowCount));
	memmove(&Array[position], &ImageData.WindowsData.WindowCount, sizeof(ImageData.WindowsData.WindowCount)); position += sizeof(ImageData.WindowsData.WindowCount);
	// Move(&ImageData.WindowsData.DataType, &Array[position], sizeof(ImageData.WindowsData.DataType));
	memmove(&Array[position], &ImageData.WindowsData.DataType, sizeof(ImageData.WindowsData.DataType)); position += sizeof(ImageData.WindowsData.DataType);
	// Move(&ImageData.WindowsData.DegreeBinning, &Array[position], sizeof(ImageData.WindowsData.DegreeBinning));
	memmove(&Array[position], &ImageData.WindowsData.DegreeBinning, sizeof(ImageData.WindowsData.DegreeBinning)); position += sizeof(ImageData.WindowsData.DegreeBinning);
	// Move(&ImageData.WindowsData.SizeData, &Array[position], sizeof(ImageData.WindowsData.SizeData));
	memmove(&Array[position], &ImageData.WindowsData.SizeData, sizeof(ImageData.WindowsData.SizeData)); position += sizeof(ImageData.WindowsData.SizeData);
	// Move(&ImageData.WindowsData.BottomRight, &Array[position], sizeof(ImageData.WindowsData.BottomRight));
	memmove(&Array[position], &ImageData.WindowsData.BottomRight, sizeof(ImageData.WindowsData.BottomRight)); position += sizeof(ImageData.WindowsData.BottomRight);
	for (int i = 0; i < ImageData.WindowsData.WindowCount; i++)
	{
		// Move(&ImageData.WindowsData.Info[i], &Array[position], sizeof(datawindow));
		memmove(&Array[position], &ImageData.WindowsData.Info[i], sizeof(datawindow)); position += sizeof(datawindow);
	}
	// Move(ImageData.WindowsData.Data, &Array[position], (MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData));
	memmove(&Array[position], ImageData.WindowsData.Data, (MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData)); position += (MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData);
	return Array;
}
// ---------------------------------------------------------------------------

void IKI_img::UnZipStructure(unsigned char* Array, unsigned int SizeInfo)
{
	unsigned int position = 0;
	unsigned int size_id; // размер пакета, без размера признака

	// Move(&Array[position], &Georeferencing.DateTime, sizeof(Georeferencing.DateTime));
	memmove(&Georeferencing.DateTime, &Array[position], sizeof(Georeferencing.DateTime)); position += sizeof(Georeferencing.DateTime);
	// Move(&Array[position], &size_id, sizeof(size_id));
	memmove(&size_id, &Array[position], sizeof(size_id)); position += sizeof(size_id); // запись длины текста
	// Move(&Array[position], &Georeferencing.SoftwareVersion[0], size_id);
	memmove(&Georeferencing.SoftwareVersion[0], &Array[position], size_id); position += size_id;
	// Move(&Array[position], &Georeferencing.FrameNumber, sizeof(Georeferencing.FrameNumber));
	memmove(&Georeferencing.FrameNumber, &Array[position], sizeof(Georeferencing.FrameNumber)); position += sizeof(Georeferencing.FrameNumber);
	// Move(&Array[position], &Georeferencing.ProtonsDensity, sizeof(Georeferencing.ProtonsDensity));
	memmove(&Georeferencing.ProtonsDensity, &Array[position], sizeof(Georeferencing.ProtonsDensity)); position += sizeof(Georeferencing.ProtonsDensity);
	// Move(&Array[position], &Georeferencing.TracksBrightness, sizeof(Georeferencing.TracksBrightness));
	memmove(&Georeferencing.TracksBrightness, &Array[position], sizeof(Georeferencing.TracksBrightness)); position += sizeof(Georeferencing.TracksBrightness);
	// Move(&Array[position], &Georeferencing.MoonVelocity, sizeof(Georeferencing.MoonVelocity));
	memmove(&Georeferencing.MoonVelocity, &Array[position], sizeof(Georeferencing.MoonVelocity)); position += sizeof(Georeferencing.MoonVelocity);
	// Move(&Array[position], &Georeferencing.MoonBrightness, sizeof(Georeferencing.MoonBrightness));
	memmove(&Georeferencing.MoonBrightness, &Array[position], sizeof(Georeferencing.MoonBrightness)); position += sizeof(Georeferencing.MoonBrightness);
	// Move(&Array[position], &Georeferencing.BrightnessLeft, sizeof(Georeferencing.BrightnessLeft));
	memmove(&Georeferencing.BrightnessLeft, &Array[position], sizeof(Georeferencing.BrightnessLeft)); position += sizeof(Georeferencing.BrightnessLeft);
	// Move(&Array[position], &Georeferencing.BrightnessRight, sizeof(Georeferencing.BrightnessRight));
	memmove(&Georeferencing.BrightnessRight, &Array[position], sizeof(Georeferencing.BrightnessRight)); position += sizeof(Georeferencing.BrightnessRight);
	// Move(&Array[position], &Georeferencing.Era, sizeof(Georeferencing.Era));
	memmove(&Georeferencing.Era, &Array[position], sizeof(Georeferencing.Era)); position += sizeof(Georeferencing.Era);
	// Move(&Array[position], &Georeferencing.OrbitEccentricity, sizeof(Georeferencing.OrbitEccentricity));
	memmove(&Georeferencing.OrbitEccentricity, &Array[position], sizeof(Georeferencing.OrbitEccentricity)); position += sizeof(Georeferencing.OrbitEccentricity);
	// Move(&Array[position], &Georeferencing.LengthSemiaxis, sizeof(Georeferencing.LengthSemiaxis));
	memmove(&Georeferencing.LengthSemiaxis, &Array[position], sizeof(Georeferencing.LengthSemiaxis)); position += sizeof(Georeferencing.LengthSemiaxis);
	// Move(&Array[position], &Georeferencing.LengthRadiusVector, sizeof(Georeferencing.LengthRadiusVector));
	memmove(&Georeferencing.LengthRadiusVector, &Array[position], sizeof(Georeferencing.LengthRadiusVector)); position += sizeof(Georeferencing.LengthRadiusVector);
	// Move(&Array[position], &Georeferencing.LongitudeAscendingNode, sizeof(Georeferencing.LongitudeAscendingNode));
	memmove(&Georeferencing.LongitudeAscendingNode, &Array[position], sizeof(Georeferencing.LongitudeAscendingNode)); position += sizeof(Georeferencing.LongitudeAscendingNode);
	// Move(&Array[position], &Georeferencing.InclinationOrbit, sizeof(Georeferencing.InclinationOrbit));
	memmove(&Georeferencing.InclinationOrbit, &Array[position], sizeof(Georeferencing.InclinationOrbit)); position += sizeof(Georeferencing.InclinationOrbit);
	// Move(&Array[position], &Georeferencing.ArgumentPerigee, sizeof(Georeferencing.ArgumentPerigee));
	memmove(&Georeferencing.ArgumentPerigee, &Array[position], sizeof(Georeferencing.ArgumentPerigee)); position += sizeof(Georeferencing.ArgumentPerigee);
	// Move(&Array[position], &Georeferencing.ArgumentLatitude, sizeof(Georeferencing.ArgumentLatitude));
	memmove(&Georeferencing.ArgumentLatitude, &Array[position], sizeof(Georeferencing.ArgumentLatitude)); position += sizeof(Georeferencing.ArgumentLatitude);
	// Move(&Array[position], &Georeferencing.DeviceAngularVelocity, sizeof(Georeferencing.DeviceAngularVelocity));
	memmove(&Georeferencing.DeviceAngularVelocity, &Array[position], sizeof(Georeferencing.DeviceAngularVelocity)); position += sizeof(Georeferencing.DeviceAngularVelocity);
	// Move(&Array[position], &Georeferencing.DeviceAngularAcceleration, sizeof(Georeferencing.DeviceAngularAcceleration));
	memmove(&Georeferencing.DeviceAngularAcceleration, &Array[position], sizeof(Georeferencing.DeviceAngularAcceleration)); position += sizeof(Georeferencing.DeviceAngularAcceleration);
	// Move(&Array[position], &Georeferencing.ProjectionLinearVelocity, sizeof(Georeferencing.ProjectionLinearVelocity));
	memmove(&Georeferencing.ProjectionLinearVelocity, &Array[position], sizeof(Georeferencing.ProjectionLinearVelocity)); position += sizeof(Georeferencing.ProjectionLinearVelocity);
	// Move(&Array[position], &Georeferencing.OrientationAngles, sizeof(Georeferencing.OrientationAngles));
	memmove(&Georeferencing.OrientationAngles, &Array[position], sizeof(Georeferencing.OrientationAngles)); position += sizeof(Georeferencing.OrientationAngles);
	// Move(&Array[position], &Georeferencing.OrientationQuaternion, sizeof(Georeferencing.OrientationQuaternion));
	memmove(&Georeferencing.OrientationQuaternion, &Array[position], sizeof(Georeferencing.OrientationQuaternion)); position += sizeof(Georeferencing.OrientationQuaternion);
	// Move(&Array[position], &Georeferencing.OrientationMatrix, sizeof(Georeferencing.OrientationMatrix));
	memmove(&Georeferencing.OrientationMatrix, &Array[position], sizeof(Georeferencing.OrientationMatrix)); position += sizeof(Georeferencing.OrientationMatrix);
	// Move(&Array[position], &Georeferencing.YearAberrationApply, sizeof(Georeferencing.YearAberrationApply));
	memmove(&Georeferencing.YearAberrationApply, &Array[position], sizeof(Georeferencing.YearAberrationApply)); position += sizeof(Georeferencing.YearAberrationApply);
	// Move(&Array[position], &Georeferencing.SatelliteAberrationApply, sizeof(Georeferencing.SatelliteAberrationApply));
	memmove(&Georeferencing.SatelliteAberrationApply, &Array[position], sizeof(Georeferencing.SatelliteAberrationApply)); position += sizeof(Georeferencing.SatelliteAberrationApply);
	// Move(&Array[position], &Georeferencing.PrecessionNutationApply, sizeof(Georeferencing.PrecessionNutationApply));
	memmove(&Georeferencing.PrecessionNutationApply, &Array[position], sizeof(Georeferencing.PrecessionNutationApply)); position += sizeof(Georeferencing.PrecessionNutationApply);
	// Move(&Array[position], &Georeferencing.PhotonNoiseApply, sizeof(Georeferencing.PhotonNoiseApply));
	memmove(&Georeferencing.PhotonNoiseApply, &Array[position], sizeof(Georeferencing.PhotonNoiseApply)); position += sizeof(Georeferencing.PhotonNoiseApply);
	// Move(&Array[position], &Georeferencing.DistortionApply, sizeof(Georeferencing.DistortionApply));
	memmove(&Georeferencing.DistortionApply, &Array[position], sizeof(Georeferencing.DistortionApply)); position += sizeof(Georeferencing.DistortionApply);

	// Move(&Array[position], &size_id, sizeof(size_id));
	memmove(&size_id, &Array[position], sizeof(size_id)); position += sizeof(size_id); // запись длины текста
	// Move(&Array[position], &CameraSettings.DataSource[0], size_id);
	memmove(&CameraSettings.DataSource[0], &Array[position], size_id); position += size_id;
	// Move(&Array[position], &CameraSettings.SNDevice, sizeof(CameraSettings.SNDevice));
	memmove(&CameraSettings.SNDevice, &Array[position], sizeof(CameraSettings.SNDevice)); position += sizeof(CameraSettings.SNDevice);
	// Move(&Array[position], &CameraSettings.InnerRadius, sizeof(CameraSettings.InnerRadius));
	memmove(&CameraSettings.InnerRadius, &Array[position], sizeof(CameraSettings.InnerRadius)); position += sizeof(CameraSettings.InnerRadius);
	// Move(&Array[position], &CameraSettings.OuterRadius, sizeof(CameraSettings.OuterRadius));
	memmove(&CameraSettings.OuterRadius, &Array[position], sizeof(CameraSettings.OuterRadius)); position += sizeof(CameraSettings.OuterRadius);
	// Move(&Array[position], &CameraSettings.ResolutionACP, sizeof(CameraSettings.ResolutionACP));
	memmove(&CameraSettings.ResolutionACP, &Array[position], sizeof(CameraSettings.ResolutionACP)); position += sizeof(CameraSettings.ResolutionACP);
	// Move(&Array[position], &CameraSettings.EV_ACP, sizeof(CameraSettings.EV_ACP));
	memmove(&CameraSettings.EV_ACP, &Array[position], sizeof(CameraSettings.EV_ACP)); position += sizeof(CameraSettings.EV_ACP);
	// Move(&Array[position], &CameraSettings.PixelSize, sizeof(CameraSettings.PixelSize));
	memmove(&CameraSettings.PixelSize, &Array[position], sizeof(CameraSettings.PixelSize)); position += sizeof(CameraSettings.PixelSize);
	// Move(&Array[position], &CameraSettings.FocalLength, sizeof(CameraSettings.FocalLength));
	memmove(&CameraSettings.FocalLength, &Array[position], sizeof(CameraSettings.FocalLength)); position += sizeof(CameraSettings.FocalLength);
	// Move(&Array[position], &CameraSettings.DiameterEntrancePupil, sizeof(CameraSettings.DiameterEntrancePupil));
	memmove(&CameraSettings.DiameterEntrancePupil, &Array[position], sizeof(CameraSettings.DiameterEntrancePupil)); position += sizeof(CameraSettings.DiameterEntrancePupil);
	// Move(&Array[position], &CameraSettings.DarkLevel, sizeof(CameraSettings.DarkLevel));
	memmove(&CameraSettings.DarkLevel, &Array[position], sizeof(CameraSettings.DarkLevel)); position += sizeof(CameraSettings.DarkLevel);
	// Move(&Array[position], &CameraSettings.DarkSKO, sizeof(CameraSettings.DarkSKO));
	memmove(&CameraSettings.DarkSKO, &Array[position], sizeof(CameraSettings.DarkSKO)); position += sizeof(CameraSettings.DarkSKO);
	// Move(&Array[position], &CameraSettings.LimitingMagnitude, sizeof(CameraSettings.LimitingMagnitude));
	memmove(&CameraSettings.LimitingMagnitude, &Array[position], sizeof(CameraSettings.LimitingMagnitude)); position += sizeof(CameraSettings.LimitingMagnitude);
	// Move(&Array[position], &CameraSettings.StarsDistribution, sizeof(CameraSettings.StarsDistribution));
	memmove(&CameraSettings.StarsDistribution, &Array[position], sizeof(CameraSettings.StarsDistribution)); position += sizeof(CameraSettings.StarsDistribution);
	// Move(&Array[position], &CameraSettings.DistortionFunction.PolynomialDegree, sizeof(CameraSettings.DistortionFunction.PolynomialDegree));
	memmove(&CameraSettings.DistortionFunction.PolynomialDegree, &Array[position], sizeof(CameraSettings.DistortionFunction.PolynomialDegree)); position += sizeof(CameraSettings.DistortionFunction.PolynomialDegree);
	// Move(&Array[position], &CameraSettings.DistortionFunction.CoefficientsCount, sizeof(CameraSettings.DistortionFunction.CoefficientsCount));
	memmove(&CameraSettings.DistortionFunction.CoefficientsCount, &Array[position], sizeof(CameraSettings.DistortionFunction.CoefficientsCount)); position += sizeof(CameraSettings.DistortionFunction.CoefficientsCount);
	_FreeMemoryArray(CameraSettings.DistortionFunction.Coefficient);
	if (CameraSettings.DistortionFunction.CoefficientsCount > 0)
	{
		CameraSettings.DistortionFunction.Coefficient = new coefonaxis[CameraSettings.DistortionFunction.CoefficientsCount];
		for (int i = 0; i < CameraSettings.DistortionFunction.CoefficientsCount; i++)
		{
			// Move(&Array[position], &CameraSettings.DistortionFunction.Coefficient[i], sizeof(coefonaxis));
			memmove(&CameraSettings.DistortionFunction.Coefficient[i], &Array[position], sizeof(coefonaxis)); position += sizeof(coefonaxis);
		}
	}

	// Move(&Array[position], &CameraSettings.TableDeadObjects.RecordsCount, sizeof(CameraSettings.TableDeadObjects.RecordsCount));
	memmove(&CameraSettings.TableDeadObjects.RecordsCount, &Array[position], sizeof(CameraSettings.TableDeadObjects.RecordsCount)); position += sizeof(CameraSettings.TableDeadObjects.RecordsCount);
	_FreeMemoryArray(CameraSettings.TableDeadObjects.PixelData);
	if (CameraSettings.TableDeadObjects.RecordsCount > 0)
	{
		CameraSettings.TableDeadObjects.PixelData = new infopix[CameraSettings.TableDeadObjects.RecordsCount];
		for (int i = 0; i < CameraSettings.TableDeadObjects.RecordsCount; i++)
		{
			// Move(&Array[position], &CameraSettings.TableDeadObjects.PixelData[i], sizeof(infopix));
			memmove(&CameraSettings.TableDeadObjects.PixelData[i], &Array[position], sizeof(infopix)); position += sizeof(infopix);
		}
	}

	// Move(&Array[position], &size_id, sizeof(size_id));
	memmove(&size_id, &Array[position], sizeof(size_id)); position += sizeof(size_id); // запись длины текста
	// Move(&Array[position], &FilterData.TagFilter[0], size_id);
	memmove(&FilterData.TagFilter[0], &Array[position], size_id); position += size_id;
	// Move(&Array[position], &FilterData.FilteredPixelsCount, sizeof(FilterData.FilteredPixelsCount));
	memmove(&FilterData.FilteredPixelsCount, &Array[position], sizeof(FilterData.FilteredPixelsCount)); position += sizeof(FilterData.FilteredPixelsCount);
	// Move(&Array[position], &FilterData.BottomRight, sizeof(FilterData.BottomRight));
	memmove(&FilterData.BottomRight, &Array[position], sizeof(FilterData.BottomRight)); position += sizeof(FilterData.BottomRight);
	_FreeMemoryArray(FilterData.DataPixels);
	if (FilterData.FilteredPixelsCount > 0)
	{
		FilterData.DataPixels = new filterdata[FilterData.FilteredPixelsCount];
		for (int i = 0; i < FilterData.FilteredPixelsCount; i++)
		{
			// Move(&Array[position], &FilterData.DataPixels[i], sizeof(filterdata));
			memmove(&FilterData.DataPixels[i], &Array[position], sizeof(filterdata)); position += sizeof(filterdata);
		}
	}

	// Move(&Array[position], &StarsData.RezStat, sizeof(StarsData.RezStat));
	memmove(&StarsData.RezStat, &Array[position], sizeof(StarsData.RezStat)); position += sizeof(StarsData.RezStat);
	// Move(&Array[position], &StarsData.Epsilon, sizeof(StarsData.Epsilon));
	memmove(&StarsData.Epsilon, &Array[position], sizeof(StarsData.Epsilon)); position += sizeof(StarsData.Epsilon);
	// Move(&Array[position], &StarsData.m_X, sizeof(StarsData.m_X));
	memmove(&StarsData.m_X, &Array[position], sizeof(StarsData.m_X)); position += sizeof(StarsData.m_X);
	// Move(&Array[position], &StarsData.m_Y, sizeof(StarsData.m_Y));
	memmove(&StarsData.m_Y, &Array[position], sizeof(StarsData.m_Y)); position += sizeof(StarsData.m_Y);
	// Move(&Array[position], &StarsData.m_Cur, sizeof(StarsData.m_Cur));
	memmove(&StarsData.m_Cur, &Array[position], sizeof(StarsData.m_Cur)); position += sizeof(StarsData.m_Cur);
	// Move(&Array[position], &StarsData.LocalizedCount, sizeof(StarsData.LocalizedCount));
	memmove(&StarsData.LocalizedCount, &Array[position], sizeof(StarsData.LocalizedCount)); position += sizeof(StarsData.LocalizedCount);
	// Move(&Array[position], &StarsData.RecognizedCount, sizeof(StarsData.RecognizedCount));
	memmove(&StarsData.RecognizedCount, &Array[position], sizeof(StarsData.RecognizedCount)); position += sizeof(StarsData.RecognizedCount);
	_FreeMemoryArray(StarsData.StarsList);
	if (StarsData.LocalizedCount > 0)
	{
		StarsData.StarsList = new starinfo[StarsData.LocalizedCount];
		for (int i = 0; i < StarsData.LocalizedCount; i++)
		{
			// Move(&Array[position], &StarsData.StarsList[i], sizeof(starinfo));
			memmove(&StarsData.StarsList[i], &Array[position], sizeof(starinfo)); position += sizeof(starinfo);
		}
	}

	// Move(&Array[position], &StarsData.SimulatedFrame.strrec, sizeof(StarsData.SimulatedFrame.strrec));
	memmove(&StarsData.SimulatedFrame.strrec, &Array[position], sizeof(StarsData.SimulatedFrame.strrec)); position += sizeof(StarsData.SimulatedFrame.strrec);
	_FreeMemoryArray(StarsData.SimulatedFrame.StarRec);
	if (StarsData.SimulatedFrame.strrec > 0)
	{
		StarsData.SimulatedFrame.StarRec = new STARREC[StarsData.SimulatedFrame.strrec];
		for (int i = 0; i < StarsData.SimulatedFrame.strrec; i++)
		{
			// Move(&Array[position], &StarsData.SimulatedFrame.StarRec[i], sizeof(starinfo));
			memmove(&StarsData.SimulatedFrame.StarRec[i], &Array[position], sizeof(starinfo)); position += sizeof(STARREC);
		}
	}
	// Move(&Array[position], &StarsData.RecognizedOrientationAngles, sizeof(StarsData.RecognizedOrientationAngles));
	memmove(&StarsData.RecognizedOrientationAngles, &Array[position], sizeof(StarsData.RecognizedOrientationAngles)); position += sizeof(StarsData.RecognizedOrientationAngles);
	// Move(&Array[position], &StarsData.RecognizedOrientationQuaternion, sizeof(StarsData.RecognizedOrientationQuaternion));
	memmove(&StarsData.RecognizedOrientationQuaternion, &Array[position], sizeof(StarsData.RecognizedOrientationQuaternion)); position += sizeof(StarsData.RecognizedOrientationQuaternion);
	// Move(&Array[position], &StarsData.RecognizedOrientationMatrix, sizeof(StarsData.RecognizedOrientationMatrix));
	memmove(&StarsData.RecognizedOrientationMatrix, &Array[position], sizeof(StarsData.RecognizedOrientationMatrix)); position += sizeof(StarsData.RecognizedOrientationMatrix);

	// Move(&Array[position], &ImageData.ExposureTime, sizeof(ImageData.ExposureTime));
	memmove(&ImageData.ExposureTime, &Array[position], sizeof(ImageData.ExposureTime)); position += sizeof(ImageData.ExposureTime);
	// Move(&Array[position], &ImageData.FrameData.DataType, sizeof(ImageData.FrameData.DataType));
	memmove(&ImageData.FrameData.DataType, &Array[position], sizeof(ImageData.FrameData.DataType)); position += sizeof(ImageData.FrameData.DataType);
	// Move(&Array[position], &ImageData.FrameData.DegreeBinning, sizeof(ImageData.FrameData.DegreeBinning));
	memmove(&ImageData.FrameData.DegreeBinning, &Array[position], sizeof(ImageData.FrameData.DegreeBinning)); position += sizeof(ImageData.FrameData.DegreeBinning);
	// Move(&Array[position], &ImageData.FrameData.FrameWidth, sizeof(ImageData.FrameData.FrameWidth));
	memmove(&ImageData.FrameData.FrameWidth, &Array[position], sizeof(ImageData.FrameData.FrameWidth)); position += sizeof(ImageData.FrameData.FrameWidth);
	// Move(&Array[position], &ImageData.FrameData.FrameHeight, sizeof(ImageData.FrameData.FrameHeight));
	memmove(&ImageData.FrameData.FrameHeight, &Array[position], sizeof(ImageData.FrameData.FrameHeight)); position += sizeof(ImageData.FrameData.FrameHeight);

	_FreeMemoryArray(ImageData.FrameData.Data);
	size_id = ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight;
	if (size_id > 0)
	{
		switch (ImageData.FrameData.DataType)
		{
		case 0: ImageData.FrameData.Data = (void*)new unsigned char[size_id]; break;
		case 1: ImageData.FrameData.Data = (void*)new unsigned short[size_id]; break;
		case 2: ImageData.FrameData.Data = (void*)new float[size_id]; break;
		}
		// Move(&Array[position], ImageData.FrameData.Data, (MassType[ImageData.FrameData.DataType] * size_id));
		memmove(&ImageData.FrameData.Data, &Array[position], (MassType[ImageData.FrameData.DataType] * size_id)); position += (MassType[ImageData.FrameData.DataType] * size_id);
	}
	// Move(&Array[position], &ImageData.LinesData.LinesCount, sizeof(ImageData.LinesData.LinesCount));
	memmove(&ImageData.LinesData.LinesCount, &Array[position], sizeof(ImageData.LinesData.LinesCount)); position += sizeof(ImageData.LinesData.LinesCount);
	// Move(&Array[position], &ImageData.LinesData.DataType, sizeof(ImageData.LinesData.DataType));
	memmove(&ImageData.LinesData.DataType, &Array[position], sizeof(ImageData.LinesData.DataType)); position += sizeof(ImageData.LinesData.DataType);
	// Move(&Array[position], &ImageData.LinesData.DegreeBinning, sizeof(ImageData.LinesData.DegreeBinning));
	memmove(&ImageData.LinesData.DegreeBinning, &Array[position], sizeof(ImageData.LinesData.DegreeBinning)); position += sizeof(ImageData.LinesData.DegreeBinning);
	// Move(&Array[position], &ImageData.LinesData.LinesWidth, sizeof(ImageData.LinesData.LinesWidth));
	memmove(&ImageData.LinesData.LinesWidth, &Array[position], sizeof(ImageData.LinesData.LinesWidth)); position += sizeof(ImageData.LinesData.LinesWidth);
	// Move(&Array[position], &ImageData.LinesData.LinesHeight, sizeof(ImageData.LinesData.LinesHeight));
	memmove(&ImageData.LinesData.LinesHeight, &Array[position], sizeof(ImageData.LinesData.LinesHeight)); position += sizeof(ImageData.LinesData.LinesHeight);
	for (int i = 0; i < ImageData.LinesData.LinesCount; i++)
	{
		// Move(&Array[position], &ImageData.LinesData.Info[i], sizeof(dataline));
		memmove(&ImageData.LinesData.Info[i], &Array[position], sizeof(dataline)); position += sizeof(dataline);
	}
	_FreeMemoryArray(ImageData.LinesData.Data);
	size_id = ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight;
	if (size_id > 0)
	{
		switch (ImageData.LinesData.DataType)
		{
		case 0: ImageData.LinesData.Data = (void*)new unsigned char[size_id]; break;
		case 1: ImageData.LinesData.Data = (void*)new unsigned short[size_id]; break;
		case 2: ImageData.LinesData.Data = (void*)new float[size_id]; break;
		}
		// Move(&Array[position], ImageData.LinesData.Data, (MassType[ImageData.LinesData.DataType] * size_id));
		memmove(&ImageData.LinesData.Data, &Array[position], (MassType[ImageData.LinesData.DataType] * size_id)); position += (MassType[ImageData.LinesData.DataType] * size_id);
	}
	// Move(&Array[position], &ImageData.WindowsData.WindowCount, sizeof(ImageData.WindowsData.WindowCount));
	memmove(&ImageData.WindowsData.WindowCount, &Array[position], sizeof(ImageData.WindowsData.WindowCount)); position += sizeof(ImageData.WindowsData.WindowCount);
	// Move(&Array[position], &ImageData.WindowsData.DataType, sizeof(ImageData.WindowsData.DataType));
	memmove(&ImageData.WindowsData.DataType, &Array[position], sizeof(ImageData.WindowsData.DataType)); position += sizeof(ImageData.WindowsData.DataType);
	// Move(&Array[position], &ImageData.WindowsData.DegreeBinning, sizeof(ImageData.WindowsData.DegreeBinning));
	memmove(&ImageData.WindowsData.DegreeBinning, &Array[position], sizeof(ImageData.WindowsData.DegreeBinning)); position += sizeof(ImageData.WindowsData.DegreeBinning);
	// Move(&Array[position], &ImageData.WindowsData.SizeData, sizeof(ImageData.WindowsData.SizeData));
	memmove(&ImageData.WindowsData.SizeData, &Array[position], sizeof(ImageData.WindowsData.SizeData)); position += sizeof(ImageData.WindowsData.SizeData);
	// Move(&Array[position], &ImageData.WindowsData.BottomRight, sizeof(ImageData.WindowsData.BottomRight));
	memmove(&ImageData.WindowsData.BottomRight, &Array[position], sizeof(ImageData.WindowsData.BottomRight)); position += sizeof(ImageData.WindowsData.BottomRight);
	for (int i = 0; i < ImageData.WindowsData.WindowCount; i++)
	{
		// Move(&Array[position], &ImageData.WindowsData.Info[i], sizeof(datawindow));
		memmove(&ImageData.WindowsData.Info[i], &Array[position], sizeof(datawindow)); position += sizeof(datawindow);
	}
	_FreeMemoryArray(ImageData.WindowsData.Data);
	if (ImageData.WindowsData.SizeData > 0)
	{
		switch (ImageData.WindowsData.DataType)
		{
		case 0: ImageData.WindowsData.Data = (void*)new unsigned char[ImageData.WindowsData.SizeData]; break;
		case 1: ImageData.WindowsData.Data = (void*)new unsigned short[ImageData.WindowsData.SizeData]; break;
		case 2: ImageData.WindowsData.Data = (void*)new float[ImageData.WindowsData.SizeData]; break;
		}
		// Move(&Array[position], ImageData.WindowsData.Data, (MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData));
		memmove(&ImageData.WindowsData.Data, &Array[position], (MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData)); position += (MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData);
	}
}
// ---------------------------------------------------------------------------
