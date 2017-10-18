#ifndef _EYEDEA_DEF_H_
#define _EYEDEA_DEF_H_

enum VisionConfigOption
{
	VISION_CONFIG_BLUR = 101,
	VISION_CONFIG_DENOISING,
	VISION_CONFIG_EDGE_LOW_THRESHOLD,
	VISION_CONFIG_EDGE_MAX_LOW_THRESHOLD,
	VISION_CONFIG_EDGE_RATIO,
	VISION_CONFIG_EDGE_KERNEL_SIZE,
	VISION_CONFIG_MORPH_SIZE,
	VISION_CONFIG_SIZE_FILTER,
	VISION_CONFIG_BASE_RECT_PADDING,
	VISION_CONFIG_DILATION_SIZE = 110,
	VISION_CONFIG_MATCHING_THRESHOLD,
	VISION_CONFIG_SEARCH_VALUE_CENTER,
	VISION_CONFIG_SEARCH_VALUE_USER_ROI,
	VISION_CONFIG_SEARCH_VALIE_AREA,
	VISION_CONFIG_SEARCH_VALUE_OBJECT,
	VISION_CONFIG_CIRCLE_DETECTION_INLIER_RATE,
	VISION_CONFIG_CIRCLE_DETECTION_MAX_ITERATION,
	VISION_CONFIG_L_MIN_ANGLE,
	VISION_CONFIG_L_MAX_ANGLE,
	VISION_CONFIG_SCENE_THRESHOLD = 120,
	VISION_CONFIG_USE_CANDIDATE_TM_UROI,
	VISION_CONFIG_USE_CANDIDATE_TM_UROI_THRESHOLD,
	VISION_CONFIG_USE_CANDIDATE_TM_AREA,
	VISION_CONFIG_USE_CANDIDATE_TM_AREA_THRESHOLD,
	VISION_CONFIG_USE_CANDIDATE_TM_OBJECT,
	VISION_CONFIG_USE_CANDIDATE_TM_OBJECT_THRESHOLD,
	VISION_CONFIG_FIND_MATER_ITERATION = 127,
	VISION_CONFIG_FIND_CENTER_SEARCH_WIDTH,
	VISION_CONFIG_FIND_CENTER_SEARCH_MAX_MATCHES,
	VISION_CONFIG_FIND_SEARCH_SEARCH_WIDTH = 130,
	VISION_CONFIG_FIND_SEARCH_SEARCH_MAX_MATCHES,
	VISION_CONFIG_FIND_MASTER_SEARCH_WIDTH,
	VISION_CONFIG_FIND_MASTER_SEARCH_MAX_MATCHES,
	VISION_CONFIG_FIND_SEARCH_CANDIDATE_SCALE,
	VISION_CONFIG_FIND_MASTER_CANDIDATE_SCALE,
	VISION_CONFIG_FIND_MASTER_CANDIDATE_PADDING,
	VISION_CONFIG_FIND_OBJECT_CANDIDATE_SCALE,
	VISION_CONFIG_CALIBRATION_CHESS_NUM_COLS,
	VISION_CONFIG_CALIBRATION_CHESS_NUM_ROWS,
	VISION_CONFIG_CALIBRATION_CHESS_SIZE = 140,
	VISION_CONFIG_FIND_OBJECT_FILER_SIZE,
	VISION_CONFIG_FIND_OBJECT_FILER_PADDING,
	VISION_CONFIG_FIND_OBJECT_LOCAL_PADDING,
	VISION_CONFIG_BLUR2,
	VISION_CONFIG_DENOISING2,
	VISION_CONFIG_EDGE_LOW_THRESHOLD2,
	VISION_CONFIG_EDGE_MAX_LOW_THRESHOLD2,
	VISION_CONFIG_EDGE_RATIO2,
	VISION_CONFIG_EDGE_KERNEL_SIZE2,
	VISION_CONFIG_MORPH_SIZE2 = 150,
	VISION_CONFIG_SIZE_FILTER2,
	VISION_CONFIG_BASE_RECT_PADDING2,
	VISION_CONFIG_DILATION_SIZE2,
	VISION_CONFIG_MATCHING_THRESHOLD2,
	VISION_CONFIG_SEARCH_VALUE_CENTER2,
	VISION_CONFIG_SEARCH_VALUE_USER_ROI2,
	VISION_CONFIG_SEARCH_VALIE_AREA2,
	VISION_CONFIG_SEARCH_VALUE_OBJECT2,
	VISION_CONFIG_CIRCLE_DETECTION_INLIER_RATE2,
	VISION_CONFIG_CIRCLE_DETECTION_MAX_ITERATION2 = 160,
	VISION_CONFIG_L_MIN_ANGLE2,
	VISION_CONFIG_L_MAX_ANGLE2,
	VISION_CONFIG_SCENE_THRESHOLD2,
	VISION_CONFIG_USE_CANDIDATE_TM_UROI2,
	VISION_CONFIG_USE_CANDIDATE_TM_UROI_THRESHOLD2,
	VISION_CONFIG_USE_CANDIDATE_TM_AREA2,
	VISION_CONFIG_USE_CANDIDATE_TM_AREA_THRESHOLD2,
	VISION_CONFIG_USE_CANDIDATE_TM_OBJECT2,
	VISION_CONFIG_USE_CANDIDATE_TM_OBJECT_THRESHOLD2,
	VISION_CONFIG_FIND_MATER_ITERATION2 = 170,
	VISION_CONFIG_EDGE_MATCHING_MARGIN,
	VISION_CONFIG_FIND_OBJECT_MASKING_OPTION,
	VISION_CONFIG_FIND_OBJECT_MASKING_OPTION_THRESHOLD,
	VISION_CONFIG_USE_CONVEXHULL,
	VISION_CONFIG_USE_BGSUB,
	SEARCHAREA_VISION_CONFIG_BLUR,
	SEARCHAREA_VISION_CONFIG_DENOISING,
	SEARCHAREA_VISION_CONFIG_EDGE_LOW_THRESHOLD,
	SEARCHAREA_VISION_CONFIG_EDGE_MAX_LOW_THRESHOLD,
	SEARCHAREA_VISION_CONFIG_EDGE_RATIO = 180,
	SEARCHAREA_VISION_CONFIG_EDGE_KERNEL_SIZE,
	SEARCHAREA_VISION_CONFIG_EDGE_MATCHING_MARGIN,
	SEARCHAREA_VISION_CONFIG_ONLY_ONE_OBJECT,
	SEARCHAREA_VISION_CONFIG_RANGE_MIN,
	SEARCHAREA_VISION_CONFIG_RANGE_MAX,
	VISION_CONFIG_FIND_OBJECT_LEVEL,
	VISION_CONFIG_USE_CALIBRATION_IMAGE,
	VISION_CONFIG_USE_HISTOGRAM,
	VISION_CONFIG_IMAGE_MORPH_SIZE,
	VISION_CONFIG_POSITIVE_RATE = 190,
	VISION_CONFIG_FIND_ONE_OF_SUBS
};

enum GetImageOption
{
	GET_IMAGE_INPUT = 201,
	GET_IMAGE_WITH_INFO,
	GET_IMAGE_BASE,
	GET_IMAGE_ZOOM_SEARCH_AREA,
	GET_IMAGE_ZOOM_MASTER_AREA,
	GET_IMAGE_RESULT,
	GET_IMAGE_BASE_WITH_INFO,
	GET_IMAGE_RESULT_FOUND_MASTER,
	GET_IMAGE_RESULT_PAP,
	GET_IMAGE_BASE_ROI,
	GET_IMAGE_CALIBRATION_ORI,
	GET_IMAGE_CALIBRATION_RESULT,
	GET_IMAGE_CALIBRATION_FEATURE,
	GET_IMAGE_LOCAL,
	GET_IMAGE_SEARCH_AREA_INFO,
	GET_IMAGE_SEARCH_AREA_FIND_RESULT,
	GET_IMAGE_BASE_HISTORY_MASK
};

enum GetBaseInformationOption
{
	GET_BASE_CENTER_X = 301,
	GET_BASE_CENTER_Y,
	GET_BASE_DEGREE,
	GET_BASE_RADIAN,
	GET_BASE_VALUE1,
	GET_BASE_VALUE2
};

enum EnvironmentOption
{
	SET_SEARCH_AREA_COLOR = 401,
	GET_SEARCH_AREA_COLOR,
	SET_SEARCH_AREA_LINE_WIDTH,
	GET_SEARCH_AREA_LINE_WIDTH,
	SET_MASTER_AREA_COLOR,
	GET_MASTER_AREA_COLOR,
	SET_MASTER_AREA_LINE_WIDTH,
	GET_MASTER_AREA_LINE_WIDTH,
	SET_OBJECT_COLOR,
	GET_OBJECT_COLOR,
	SET_OBJECT_LINE_WIDTH,
	GET_OBJECT_LINE_WIDTH,
	GET_OBJECT_USE_FEATURE,
	GET_OBJECT_FEATURE_COLOR,
	GET_OBJECT_FEATURE_WIDTH,
	SET_OBJECT_USE_FEATURE,
	SET_OBJECT_FEATURE_COLOR,
	SET_OBJECT_FEATURE_WIDTH,
	GET_OBJECT_USE_OUTLINE,
	GET_OBJECT_OUTLINE_COLOR,
	GET_OBJECT_OUTLINE_WIDTH,
	SET_OBJECT_USE_OUTLINE,
	SET_OBJECT_OUTLINE_COLOR,
	SET_OBJECT_OUTLINE_WIDTH,
	GET_OBJECT_USE_RECT,
	GET_OBJECT_RECT_COLOR,
	GET_OBJECT_RECT_WIDTH,
	SET_OBJECT_USE_RECT,
	SET_OBJECT_RECT_COLOR,
	SET_OBJECT_RECT_WIDTH,
	GET_OBJECT_USE_CENTER,
	GET_OBJECT_CENTER_COLOR,
	GET_OBJECT_CENTER_WIDTH,
	SET_OBJECT_USE_CENTER,
	SET_OBJECT_CENTER_COLOR,
	SET_OBJECT_CENTER_WIDTH,
	GET_OBJECT_USE_ANGLE,
	GET_OBJECT_ANGLE_COLOR,
	GET_OBJECT_ANGLE_WIDTH,
	SET_OBJECT_USE_ANGLE,
	SET_OBJECT_ANGLE_COLOR,
	SET_OBJECT_ANGLE_WIDTH
};

enum DrawResultOption
{
	RESULT_DRAW_OPTION_SEARCH_FEAT	= 0x0001,
	RESULT_DRAW_OPTION_SEARCH_BOX	= 0x0002,
	RESULT_DRAW_OPTION_SEARCH_ARROW	= 0x0004,
	RESULT_DRAW_OPTION_MASTER_FEAT	= 0x0008,
	RESULT_DRAW_OPTION_MASTER_BOX	= 0x0010,
	RESULT_DRAW_OPTION_MASTER_TEXT	= 0x0020,
	RESULT_DRAW_OPTION_OBJECT		= 0x0040,
	RESULT_DRAW_OPTION_OBJECT_TEXT	= 0x0080,
	WITH_DRAW_OPTION_FEATURE		= 0x0100,
	WITH_DRAW_SEARCH_AREA_RANGE		= 0x0200,
	RESULT_DRAW_OPTION_ALL = 0xFFFF
};

enum GetResultInfoList
{
	GET_RESULT_SEARCH_AREA_CAMERA_BASE = 501,
	GET_RESULT_MASTER_AREA_CAMERA_BASE,
	GET_RESULT_SCALE,
	GET_RESULT_SCORE,
	GET_RESULT_SEARCH_AREA_ROBOT_BASE,
	GET_RESULT_MASTER_AREA_ROBOT_BASE,
	GET_RESULT_SEARCH_AREA_IMAGE_BASE,
	GET_RESULT_MASTER_AREA_IMAGE_BASE
};

enum ERVSMode
{
	ERVS_MODE_NORMAL = 0,
	ERVS_MODE_CALIBRATION
};

enum GetDBInfoOption
{
	GET_DB_SUB_INFO_ID_BY_IDINDEX = 601,
	GET_DB_SUB_INFO_SEARCHAREA_BY_IDINDEX,
	GET_DB_SUB_INFO_MASTERAREA_BY_IDINDEX,
	GET_DB_SUB_INFO_USE_INSPECTION_BY_IDINDEX,
	GET_DB_SUB_INFO_CIRCLE_BY_IDINDEX,
	GET_DB_SUB_INFO_SEARCHAREA_BY_IDID,
	GET_DB_SUB_INFO_MASTERAREA_BY_IDID,
	GET_DB_SUB_INFO_USE_INSPECTION_BY_IDID,
	GET_DB_SUB_INFO_CIRCLE_BY_IDID
};

enum SetCameraConfigOption
{
	SET_CAMERA_AUTO_EXPOSURE = 701,
	SET_CAMERA_AUTO_EXPOSURE_RANGE_GO_OUT,
	SET_CAMERA_AUTO_EXPOSURE_RANGE_ENTER,
	SET_CAMERA_MANUAL_EXPOSURE,
	SET_CAMERA_MANUAL_EXPOSURE_EXPOSURE,
	SET_CAMERA_MANUAL_EXPOSURE_GAIN,
	SET_CAMERA_TAKE_PIC,
	SET_CAMERA_LED_BRIGHTNESS,
	SET_CAMERA_LED_TIMER_ON,
};

enum GetCameraConfigOption
{
	GET_CAMERA_AUTO_EXPOSURE = 801,
	GET_CAMERA_AUTO_EXPOSURE_RANGE_GO_OUT,
	GET_CAMERA_AUTO_EXPOSURE_RANGE_ENTER,
	GET_CAMERA_MANUAL_EXPOSURE,
	GET_CAMERA_MANUAL_EXPOSURE_EXPOSURE,
	GET_CAMERA_MANUAL_EXPOSURE_GAIN,
	GET_CAMERA_LED_BRIGHTNESS,
};

enum HistogramUseOption
{
	HISTOGRAM_USE_GRAY = 0x0001,
	HISTOGRAM_USE_RED = 0x0002,
	HISTOGRAM_USE_GREEN = 0x0004,
	HISTOGRAM_USE_BLUE = 0x0008,
	HISTOGRAM_USE_ALL = 0xFFFF
};

#endif