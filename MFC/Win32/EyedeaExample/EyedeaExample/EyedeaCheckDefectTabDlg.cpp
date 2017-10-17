// EyedeaCheckDefectTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EyedeaExample.h"
#include "EyedeaCheckDefectTabDlg.h"
#include "afxdialogex.h"
#include "EyedeaExampleDlg.h"

// CEyedeaCheckDefectTabDlg dialog

IMPLEMENT_DYNAMIC(CEyedeaCheckDefectTabDlg, CDialogEx)

CEyedeaCheckDefectTabDlg::CEyedeaCheckDefectTabDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_OBEJCT_DETECT_AND_CHECK_DEFECT, pParent)
	, m_i_checkdefect_ret(0)
	, m_run_thread(false)
	, m_b_draw_pause(true)
	, m_histogram_size(0.0)
	, m_p_histogram(NULL)
	, m_p_histogram_r(NULL)
	, m_p_histogram_g(NULL)
	, m_p_histogram_b(NULL)
	, m_i_histogram_lbdwn_object(-1)
	, m_i_histogmra_gray_min(-1)
	, m_i_histogmra_gray_max(-1)
	, m_i_histogmra_red_min(-1)
	, m_i_histogmra_red_max(-1)
	, m_i_histogmra_green_min(-1)
	, m_i_histogmra_green_max(-1)
	, m_i_histogmra_blue_min(-1)
	, m_i_histogmra_blue_max(-1)
	, m_f_histogram_w_scale_v(0)
	, m_f_histogram_w_scale_r(0)
	, m_f_histogram_w_scale_g(0)
	, m_f_histogram_w_scale_b(0)
	, m_f_histogram_size_v_on_ui(0)
	, m_f_histogram_size_r_on_ui(0)
	, m_f_histogram_size_g_on_ui(0)
	, m_f_histogram_size_b_on_ui(0)
	, m_f_start_x_v_on_ui(0)
	, m_f_start_x_r_on_ui(0)
	, m_f_start_x_g_on_ui(0)
	, m_f_start_x_b_on_ui(0)
	, m_select_id(0)
{
	m_result_image = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
	m_result_histogram_image = cv::Mat::zeros(cv::Size(300, 200), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
	m_result_histogram_r_image = cv::Mat::zeros(cv::Size(300, 200), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
	m_result_histogram_g_image = cv::Mat::zeros(cv::Size(300, 200), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
	m_result_histogram_b_image = cv::Mat::zeros(cv::Size(300, 200), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
}

CEyedeaCheckDefectTabDlg::~CEyedeaCheckDefectTabDlg()
{
	m_ImgList.DeleteImageList();

	if (m_p_histogram != NULL)
	{
		free(m_p_histogram);
		m_p_histogram = NULL;
	}

	if (m_p_histogram_r != NULL)
	{
		free(m_p_histogram_r);
		m_p_histogram_r = NULL;
	}

	if (m_p_histogram_g != NULL)
	{
		free(m_p_histogram_g);
		m_p_histogram_g = NULL;
	}

	if (m_p_histogram_b != NULL)
	{
		free(m_p_histogram_b);
		m_p_histogram_b = NULL;
	}

	m_b_draw_pause = true;
	m_run_thread = false;
}

void CEyedeaCheckDefectTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO_OBJECT_TYPE, m_combo_object_type);
	//DDX_Control(pDX, IDC_LIST_FIND_OBJECTS, m_list_find_objects);
	//DDX_Control(pDX, IDC_COMBO_DEPENDENCY_LIST, m_combo_dependency_list);
	//DDX_Control(pDX, IDC_BUTTON_SELECT_BASE, m_btn_select_base_object);
	DDX_Control(pDX, IDC_RICHEDIT2_PRINT, m_editMain);
	//DDX_Control(pDX, IDC_LIST_RESULT, m_list_result_index);
	DDX_Control(pDX, IDC_TREE_RESULT, m_tree_result);
}


BEGIN_MESSAGE_MAP(CEyedeaCheckDefectTabDlg, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON_SET_BASE, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonSetBase)
	//ON_BN_CLICKED(IDC_CHECK_USE_PCA, &CEyedeaCheckDefectTabDlg::OnBnClickedCheckUsePca)
	//ON_BN_CLICKED(IDC_CHECK_OPTION_FIX_SEARCH_AREA, &CEyedeaCheckDefectTabDlg::OnBnClickedCheckOptionFixSearchArea)
	//ON_BN_CLICKED(IDC_BUTTON_SELECT_SEARCH_AREA, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonSelectSearchArea)
	//ON_BN_CLICKED(IDC_BUTTON_VISION_CONFIG_FIND_SEARCH_AREA_GET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindSearchAreaGet)
	//ON_BN_CLICKED(IDC_BUTTON_VISION_CONFIG_FIND_SEARCH_AREA_SET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindSearchAreaSet)
	//ON_BN_CLICKED(IDC_BUTTON_SELECT_MASTER_AREA, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonSelectMasterArea)
	//ON_BN_CLICKED(IDC_BUTTON_VISION_CONFIG_FIND_ITERATION_GET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindIterationGet)
	//ON_BN_CLICKED(IDC_BUTTON_VISION_CONFIG_FIND_ITERATION_SET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindIterationSet)
	//ON_BN_CLICKED(IDC_BUTTON_ADD_OBJECT, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonAddObject)
	//ON_BN_CLICKED(IDC_BUTTON_VISION_CONFIG_FIND_CENTER_GET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindCenterGet)
	//ON_BN_CLICKED(IDC_BUTTON_VISION_CONFIG_FIND_CENTER_SET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindCenterSet)
	//ON_BN_CLICKED(IDC_BUTTON_FIND, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFind)
	//ON_BN_CLICKED(IDC_CHECK_USE_FIND_WITH_CHECKDEFECT, &CEyedeaCheckDefectTabDlg::OnBnClickedCheckUseFindWithCheckdefect)
	//ON_BN_CLICKED(IDC_BUTTON_VISION_CONFIG_FIND_OBJECT_GET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindObjectGet)
	//ON_BN_CLICKED(IDC_BUTTON_VISION_CONFIG_FIND_OBJECT_SET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindObjectSet)
	//ON_BN_CLICKED(IDC_BUTTON_SELECT_BASE, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonSelectBase)
	//ON_BN_CLICKED(IDC_CHECK_FIND_MULTI_OBJECTS, &CEyedeaCheckDefectTabDlg::OnBnClickedCheckFindMultiObjects)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT_FILTER_SIZE_GET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindObjectFilterSizeGet)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT_FILTER_PADDING_GET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindObjectFilterPaddingGet)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT_FILTER_SIZE_SET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindObjectFilterSizeSet)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT_FILTER_PADDING_SET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindObjectFilterPaddingSet)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_GET_INFO, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindGetInfo)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT_LOCAL_PADDING_GET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindObjectLocalPaddingGet)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT_LOCAL_PADDING_SET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindObjectLocalPaddingSet)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_GET_POSE, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindGetPose)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT_CLASSIFIER_THRE_GET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindObjectClassifierThreGet)
	ON_BN_CLICKED(IDC_BUTTON_FIND_GET_INFO2, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindGetInfo2)
	//ON_BN_CLICKED(IDC_BUTTON_CHECK_CAMERA_CALIBOK, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonCheckCameraCalibok)
	//ON_BN_CLICKED(IDC_CHECK_DEPENDENCY_SEARCHAREA, &CEyedeaCheckDefectTabDlg::OnBnClickedCheckDependencySearcharea)
	//ON_BN_CLICKED(IDC_BUTTON_SET_ZOOM_IMAGE, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonSetZoomImage)
	//ON_BN_CLICKED(IDC_BUTTON_RESET_ZOOM_IMAGE, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonResetZoomImage)
	//ON_BN_CLICKED(IDC_BUTTON_DEP_ID_CHECK, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonDepIdCheck)
	//ON_BN_CLICKED(IDC_CHECK_FIXED_AREA, &CEyedeaCheckDefectTabDlg::OnBnClickedCheckFixedArea)
	//ON_BN_CLICKED(IDC_BUTTON_SELECT_CIRCLE, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonSelectCircle)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT_DETECTION_LEVEL_GET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindObjectDetectionLevelGet)
	//ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT_DETECTION_LEVEL_SET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindObjectDetectionLevelSet)
	//ON_BN_CLICKED(IDC_CHECK_USE_CALIBRATION_IMAGE, &CEyedeaCheckDefectTabDlg::OnBnClickedCheckUseCalibrationImage)
	ON_BN_CLICKED(IDC_CHECK_HISTOGRAM, &CEyedeaCheckDefectTabDlg::OnBnClickedCheckHistogram)
	//ON_LBN_DBLCLK(IDC_LIST_RESULT, &CEyedeaCheckDefectTabDlg::OnLbnDblclkListResult)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_RESULT, &CEyedeaCheckDefectTabDlg::OnNMDblclkTreeResult)
	//ON_BN_CLICKED(IDC_CHECK_ONE_OF_SUBS, &CEyedeaCheckDefectTabDlg::OnBnClickedCheckOneOfSubs)
	ON_BN_CLICKED(IDC_BUTTON2, &CEyedeaCheckDefectTabDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_GEO_ANGLE, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonGeoAngle)
	ON_BN_CLICKED(IDC_BUTTON_GEO_RESULT_CLEAR, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonGeoResultClear)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_HISTO_GET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoGet)
	ON_BN_CLICKED(IDC_BUTTON_HISTO_GET2, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoGet2)
	ON_BN_CLICKED(IDC_BUTTON_HISTO_GET3, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoGet3)
	ON_BN_CLICKED(IDC_BUTTON_HISTO_GET4, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoGet4)
	ON_BN_CLICKED(IDC_BUTTON_HISTO_SET, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoSet)
	ON_BN_CLICKED(IDC_BUTTON_HISTO_SET2, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoSet2)
	ON_BN_CLICKED(IDC_BUTTON_HISTO_SET3, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoSet3)
	ON_BN_CLICKED(IDC_BUTTON_HISTO_SET4, &CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoSet4)
END_MESSAGE_MAP()


// CEyedeaCheckDefectTabDlg message handlers

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonSetBase()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_DEP_ID)->GetWindowText(str);
	int dep_id = _ttoi(str);

	ERVS_SetBase(dep_id);

	//((CEyedeaExampleDlg *)GetParent())->m_command = USER_COMMAND_NORMAL;
	m_command = USER_COMMAND_NORMAL;

	//check for single mode
	OnBnClickedCheckDependencySearcharea();
}
#endif

/*
void CEyedeaCheckDefectTabDlg::OnBnClickedCheckUsePca()
{
	// TODO: Add your control notification handler code here
	BOOL bCheckOptionPCA = IsDlgButtonChecked(IDC_CHECK_USE_PCA);

	if (bCheckOptionPCA)
	{
		ERVS_SetOptionPCAOn();
	}
	else
	{
		ERVS_SetOptionPCAOff();
	}

	//Get
	int ret = ERVS_GetOptionPCA();

	if (ret > 0)
	{
		CheckDlgButton(IDC_CHECK_USE_PCA, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_USE_PCA, FALSE);
	}
}
*/

void CEyedeaCheckDefectTabDlg::MyTextOut(LPCTSTR strText, COLORREF TextColor)
{
	// 마지막 행으로 이동하기
	int first_pos = m_editMain.LineIndex(m_editMain.GetLineCount());
	m_editMain.SetSel(first_pos, first_pos);
	CPoint point;
	point = m_editMain.PosFromChar(first_pos);
	m_editMain.SetCaretPos(point);
	m_editMain.SetFocus();

	// 문자출력
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = TextColor;
	m_editMain.SetSelectionCharFormat(cf);
	m_editMain.ReplaceSel(strText);
}

BOOL CEyedeaCheckDefectTabDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//m_btn_select_base_object.SetIcon(AfxGetApp()->LoadIcon(IDB_BITMAP2));

	//AfxInitRichEdit();

#if 0
	m_ImgList.Create(160, 120, ILC_COLOR24, 0, 0); //이미지 아이콘의 크기를 결정하는듯			
	m_list_find_objects.SetImageList(&m_ImgList, LVSIL_NORMAL);
#endif

	//m_combo_object_type.AddString(_T("Circle"));
	//m_combo_object_type.SetCurSel(0);

	m_mat_icon = cv::imread("icons1_0.png");

	if (!m_mat_icon.empty())
	{
		m_mat_icon(cv::Rect(0, 0, 404, 417)).copyTo(m_mat_pass);
		m_mat_icon(cv::Rect(404, 0, 404, 417)).copyTo(m_mat_fail);
		m_mat_icon(cv::Rect(404 * 2, 0, 404, 417)).copyTo(m_mat_warning);
	}
	else
	{
		m_mat_pass = cv::Mat::zeros(cv::Size(404, 417), CV_8UC3);
		m_mat_pass = cv::Scalar(0, 255, 0);
		m_mat_fail = cv::Mat::zeros(cv::Size(404, 417), CV_8UC3);
		m_mat_fail = cv::Scalar(0, 0, 255);
		m_mat_warning = cv::Mat::zeros(cv::Size(404, 417), CV_8UC3);
		m_mat_warning = cv::Scalar(255, 0, 0);
	}

	//m_combo_dependency_list.AddString(_T("No"));
	//m_combo_dependency_list.SetCurSel(0);
#if 0
	//Initialize list
	m_list_find_objects.DeleteAllItems();
	// 리스트 스타일 설정
	m_list_find_objects.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 타이틀 삽입
	m_list_find_objects.InsertColumn(0, _T("Idx"), LVCFMT_CENTER, 30, -1);
	m_list_find_objects.InsertColumn(1, _T("ID"), LVCFMT_CENTER, 50, -1);
	m_list_find_objects.InsertColumn(2, _T("cX"), LVCFMT_CENTER, 150, -1);
	m_list_find_objects.InsertColumn(3, _T("cY"), LVCFMT_CENTER, 150, -1);
	m_list_find_objects.InsertColumn(4, _T("cA"), LVCFMT_CENTER, 100, -1);
	m_list_find_objects.InsertColumn(5, _T("score"), LVCFMT_CENTER, 70, -1);
#endif

	CString str;
	str.Format(_T("10"));
	GetDlgItem(IDC_EDIT_FIND_MAX_COUNT)->SetWindowText(str);

	//start Thread
	//eyedea - start of thread
	m_run_thread = true;
	//cread thread
	m_thread = boost::thread(boost::bind(&CEyedeaCheckDefectTabDlg::ThreadFunctionDraw, this));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedCheckOptionFixSearchArea()
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_OPTION_FIX_SEARCH_AREA);

	if (bCheck)
	{
		ERVS_OptionFixSearchAreaOn();
	}
	else
	{
		ERVS_OptionFixSearchAreaOff();
	}

	int ret = ERVS_GetOptionFixSearchArea();

	if (ret > 0)
	{
		CheckDlgButton(IDC_CHECK_OPTION_FIX_SEARCH_AREA, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_OPTION_FIX_SEARCH_AREA, FALSE);
	}
}
#endif


#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonSelectSearchArea()
{
	// TODO: Add your control notification handler code here
	((CEyedeaExampleDlg *)GetParent())->m_combo_get_image_option.SetCurSel(1);		//Set Base Image

	((CEyedeaExampleDlg *)GetParent())->m_command = USER_COMMAND_SELECT_SEARCH_AREA; 
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindSearchAreaGet()
{
	// TODO: Add your control notification handler code here
	float search_search_width = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_SEARCH_SEARCH_WIDTH);	
	float search_search_max_matches = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_SEARCH_SEARCH_MAX_MATCHES);
	float search_search_candidate_scale = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_SEARCH_CANDIDATE_SCALE);

	CString str;
	str.Format(_T("%.2f"), search_search_width);
	GetDlgItem(IDC_EDIT_FIND_OPTION_SEARCH_SEARCH_WIDTH)->SetWindowText(str);

	str.Format(_T("%.2f"), search_search_max_matches);
	GetDlgItem(IDC_EDIT_FIND_OPTION_SEARCH_MAX_MATCHES)->SetWindowText(str);

	str.Format(_T("%.2f"), search_search_candidate_scale);
	GetDlgItem(IDC_EDIT_FIND_OPTION_SEARCH_CANDIDATE_SCALE)->SetWindowText(str);

	int ret = ERVS_GetOptionFixSearchArea();

	if (ret)
	{
		CheckDlgButton(IDC_CHECK_OPTION_FIX_SEARCH_AREA, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_OPTION_FIX_SEARCH_AREA, FALSE);
	}
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindSearchAreaSet()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_FIND_OPTION_SEARCH_SEARCH_WIDTH)->GetWindowText(str);
	float search_search_width = _ttof(str);

	//set
	ERVS_SetVisionConfigOption(VISION_CONFIG_FIND_SEARCH_SEARCH_WIDTH, search_search_width);

	search_search_width = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_SEARCH_SEARCH_WIDTH);

	str.Format(_T("%.2f"), search_search_width);
	GetDlgItem(IDC_EDIT_FIND_OPTION_SEARCH_SEARCH_WIDTH)->SetWindowText(str);

	//

	GetDlgItem(IDC_EDIT_FIND_OPTION_SEARCH_MAX_MATCHES)->GetWindowText(str);
	float search_search_max_matches = _ttof(str);

	//set
	ERVS_SetVisionConfigOption(VISION_CONFIG_FIND_SEARCH_SEARCH_MAX_MATCHES, search_search_max_matches);

	search_search_max_matches = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_SEARCH_SEARCH_MAX_MATCHES);

	str.Format(_T("%.2f"), search_search_max_matches);
	GetDlgItem(IDC_EDIT_FIND_OPTION_SEARCH_MAX_MATCHES)->SetWindowText(str);
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonSelectMasterArea()
{
	// TODO: Add your control notification handler code here
	((CEyedeaExampleDlg *)GetParent())->m_combo_get_image_option.SetCurSel(3);		//Set Search Image

	((CEyedeaExampleDlg *)GetParent())->m_command = USER_COMMAND_SELECT_MASTER_AREA;

}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindIterationGet()
{
	// TODO: Add your control notification handler code here
	float master_search_width = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_MASTER_SEARCH_WIDTH);
	float master_search_max_matches = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_MASTER_SEARCH_MAX_MATCHES);
	float master_search_candidate_scale = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_MASTER_CANDIDATE_SCALE);
	float master_search_candidate_padding = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_MASTER_CANDIDATE_PADDING);

	CString str;

	str.Format(_T("%.2f"), master_search_candidate_scale);
	GetDlgItem(IDC_EDIT_FIND_OPTION_MASTER_CANDIDATE_SCALE)->SetWindowText(str);

	str.Format(_T("%.2f"), master_search_candidate_padding);
	GetDlgItem(IDC_EDIT_FIND_OPTION_MASTER_CANDIDATE_PADDING)->SetWindowText(str);
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindIterationSet()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_FIND_OPTION_MASTER_CANDIDATE_PADDING)->GetWindowText(str);
	float master_search_candidate_padding = _ttof(str);

	//set
	ERVS_SetVisionConfigOption(VISION_CONFIG_FIND_MASTER_CANDIDATE_PADDING, master_search_candidate_padding);

	master_search_candidate_padding = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_MASTER_CANDIDATE_PADDING);

	str.Format(_T("%.2f"), master_search_candidate_padding);
	GetDlgItem(IDC_EDIT_FIND_OPTION_MASTER_CANDIDATE_PADDING)->SetWindowText(str);
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonAddObject()
{
	// TODO: Add your control notification handler code here
	((CEyedeaExampleDlg *)GetParent())->m_combo_get_image_option.SetCurSel(4);		//Set Master Image

	((CEyedeaExampleDlg *)GetParent())->m_command = USER_COMMAND_SELECT_OBJECT;
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindCenterGet()
{
	// TODO: Add your control notification handler code here
	float center_search_width = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_CENTER_SEARCH_WIDTH);

	CString str;

	float center_search_max_matches = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_CENTER_SEARCH_MAX_MATCHES);

}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindCenterSet()
{
	// TODO: Add your control notification handler code here
	CString str;

	//set
	ERVS_SetVisionConfigOption(VISION_CONFIG_FIND_CENTER_SEARCH_WIDTH, center_search_width);

	center_search_width = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_CENTER_SEARCH_WIDTH);
	//


	//set
	ERVS_SetVisionConfigOption(VISION_CONFIG_FIND_CENTER_SEARCH_MAX_MATCHES, center_search_max_matches);

	center_search_max_matches = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_CENTER_SEARCH_MAX_MATCHES);
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonFind()
{
	// TODO: Add your control notification handler code here
	ERVS_FindObject();

	((CEyedeaExampleDlg *)GetParent())->m_combo_get_image_option.SetCurSel(5);		//set result


	int isCalibOK = ERVS_Calibration_isOK();

	CString unit; // = "pixel";
	unit.Format(_T("pixel"));
	if( isCalibOK )	unit.Format(_T("mm"));

	//camera base			
	//Result Info
	float search_x1 = 0.0, search_y1 = 0.0, search_x2 = 0.0, search_y2 = 0.0, search_x3 = 0.0, search_y3 = 0.0, search_x4 = 0.0, search_y4 = 0.0, search_angle = 0.0;
	ERVS_GetResultFindSearchAreaCameraBase(&search_x1, &search_y1, &search_x2, &search_y2, &search_x3, &search_y3, &search_x4, &search_y4, &search_angle);

	float master_x1 = 0.0, master_y1 = 0.0, master_x2 = 0.0, master_y2 = 0.0, master_x3 = 0.0, master_y3 = 0.0, master_x4 = 0.0, master_y4 = 0.0, master_angle = 0.0;
	ERVS_GetResultFindMasterAreaCameraBase(&master_x1, &master_y1, &master_x2, &master_y2, &master_x3, &master_y3, &master_x4, &master_y4, &master_angle);

	float scale = ERVS_GetResultFindScale();
	float score = ERVS_GetResultFindScore();

	//robot base
	float robot_search_x1 = 0.0, robot_search_y1 = 0.0, robot_search_x2 = 0.0, robot_search_y2 = 0.0, robot_search_x3 = 0.0, robot_search_y3 = 0.0, robot_search_x4 = 0.0, robot_search_y4 = 0.0, robot_search_angle = 0.0;
	ERVS_GetResultFindSearchAreaRobotBase(&robot_search_x1, &robot_search_y1, &robot_search_x2, &robot_search_y2, &robot_search_x3, &robot_search_y3, &robot_search_x4, &robot_search_y4, &robot_search_angle);

	float robot_master_x1 = 0.0, robot_master_y1 = 0.0, robot_master_x2 = 0.0, robot_master_y2 = 0.0, robot_master_x3 = 0.0, robot_master_y3 = 0.0, robot_master_x4 = 0.0, robot_master_y4 = 0.0, robot_master_angle = 0.0;
	ERVS_GetResultFindMasterAreaRobotBase(&robot_master_x1, &robot_master_y1, &robot_master_x2, &robot_master_y2, &robot_master_x3, &robot_master_y3, &robot_master_x4, &robot_master_y4, &robot_master_angle);

	//image base
	float image_search_x1 = 0.0, image_search_y1 = 0.0, image_search_x2 = 0.0, image_search_y2 = 0.0, image_search_x3 = 0.0, image_search_y3 = 0.0, image_search_x4 = 0.0, image_search_y4 = 0.0, image_search_angle = 0.0;
	ERVS_GetResultFindSearchAreaImageBase(&image_search_x1, &image_search_y1, &image_search_x2, &image_search_y2, &image_search_x3, &image_search_y3, &image_search_x4, &image_search_y4, &image_search_angle);

	float image_master_x1 = 0.0, image_master_y1 = 0.0, image_master_x2 = 0.0, image_master_y2 = 0.0, image_master_x3 = 0.0, image_master_y3 = 0.0, image_master_x4 = 0.0, image_master_y4 = 0.0, image_master_angle = 0.0;
	ERVS_GetResultFindMasterAreaImageBase(&image_master_x1, &image_master_y1, &image_master_x2, &image_master_y2, &image_master_x3, &image_master_y3, &image_master_x4, &image_master_y4, &image_master_angle);

	//to UI
	CString strText;
	strText.Format(_T("%.2f , %.2f (%s)"), master_x1, master_y1, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_1)->SetWindowText(strText);
	strText.Format(_T("%.2f , %.2f (%s)"), master_x2, master_y2, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_2)->SetWindowText(strText);
	strText.Format(_T("%.2f , %.2f (%s)"), master_x3, master_y3, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_3)->SetWindowText(strText);
	strText.Format(_T("%.2f , %.2f (%s)"), master_x4, master_y4, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_4)->SetWindowText(strText);

	//center
	double center_camera_base_x = (master_x1 + master_x2 + master_x3 + master_x4) / 4.0;
	double center_camera_base_y = (master_y1 + master_y2 + master_y3 + master_y4) / 4.0;
	strText.Format(_T("%.2f , %.2f (%s)"), center_camera_base_x, center_camera_base_y, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_CENTER_CAMERA_BASE)->SetWindowText(strText);

	strText.Format(_T("%.2f , %.2f (%s)"), robot_master_x1, robot_master_y1, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_5)->SetWindowText(strText);
	strText.Format(_T("%.2f , %.2f (%s)"), robot_master_x2, robot_master_y2, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_6)->SetWindowText(strText);
	strText.Format(_T("%.2f , %.2f (%s)"), robot_master_x3, robot_master_y3, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_7)->SetWindowText(strText);
	strText.Format(_T("%.2f , %.2f (%s)"), robot_master_x4, robot_master_y4, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_8)->SetWindowText(strText);

	//center
	double center_robot_base_x = (robot_master_x1 + robot_master_x2 + robot_master_x3 + robot_master_x4) / 4.0;
	double center_robot_base_y = (robot_master_y1 + robot_master_y2 + robot_master_y3 + robot_master_y4) / 4.0;
	strText.Format(_T("%.2f , %.2f (%s)"), center_robot_base_x, center_robot_base_y, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_CENTER_ROBOT_BASE)->SetWindowText(strText);

	strText.Format(_T("%.2f , %.2f (pixel)"), image_master_x1, image_master_y1);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_9)->SetWindowText(strText);
	strText.Format(_T("%.2f , %.2f (pixel)"), image_master_x2, image_master_y2);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_10)->SetWindowText(strText);
	strText.Format(_T("%.2f , %.2f (pixel)"), image_master_x3, image_master_y3);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_11)->SetWindowText(strText);
	strText.Format(_T("%.2f , %.2f (pixel)"), image_master_x4, image_master_y4);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_12)->SetWindowText(strText);

	//center
	double center_image_base_x = (image_master_x1 + image_master_x2 + image_master_x3 + image_master_x4) / 4.0;
	double center_image_base_y = (image_master_y1 + image_master_y2 + image_master_y3 + image_master_y4) / 4.0;
	strText.Format(_T("%.2f , %.2f (%s)"), center_image_base_x, center_image_base_y, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_MASTER_CENTER_IMAGE_BASE)->SetWindowText(strText);

	strText.Format(_T("%.2f"), scale);
	GetDlgItem(IDC_EDIT_FIND_RESULT_SCALE)->SetWindowText(strText);

	strText.Format(_T("%.2f"), score);
	GetDlgItem(IDC_EDIT_FIND_RESULT_SCORE)->SetWindowText(strText);

	//object
	float camera_circle_x = 0.0, camera_circle_y = 0.0, camera_circle_r = 0.0;
	ERVS_GetResultCircleInfoCameraBase(&camera_circle_x, &camera_circle_y, &camera_circle_r);
	float robot_circle_x = 0.0, robot_circle_y = 0.0, robot_circle_r = 0.0;
	ERVS_GetResultCircleInfoRobotBase(&robot_circle_x, &robot_circle_y, &robot_circle_r);
	float image_circle_x = 0.0, image_circle_y = 0.0, image_circle_r = 0.0;
	ERVS_GetResultCircleInfoImageBase(&image_circle_x, &image_circle_y, &image_circle_r);

	//camera
	strText.Format(_T("%.2f , %.2f (%s)"), camera_circle_x, camera_circle_y, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_OBJECT_CIRCLE_CENTER)->SetWindowText(strText);
	strText.Format(_T("%.2f (%s)"), camera_circle_r, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_OBJECT_CIRCLE_RADIUS)->SetWindowText(strText);

	//robot
	strText.Format(_T("%.2f , %.2f (%s)"), robot_circle_x, robot_circle_y, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_OBJECT_CIRCLE_CENTER2)->SetWindowText(strText);
	strText.Format(_T("%.2f (%s)"), robot_circle_r, unit);
	GetDlgItem(IDC_EDIT_FIND_RESULT_OBJECT_CIRCLE_RADIUS2)->SetWindowText(strText);

	//image
	strText.Format(_T("%.2f , %.2f (pixel)"), image_circle_x, image_circle_y);
	GetDlgItem(IDC_EDIT_FIND_RESULT_OBJECT_CIRCLE_CENTER3)->SetWindowText(strText);
	strText.Format(_T("%.2f (pixel)"), image_circle_r);
	GetDlgItem(IDC_EDIT_FIND_RESULT_OBJECT_CIRCLE_RADIUS3)->SetWindowText(strText);



	BOOL bCheckOptionDefect = IsDlgButtonChecked(IDC_CHECK_USE_FIND_WITH_CHECKDEFECT);

	if (bCheckOptionDefect)
	{
		CString str;
		GetDlgItem(IDC_EDIT_FIND_WITH_CHECK_DEFECT_THRE)->GetWindowText(str);
		float threshold = _ttof(str);

		int ret = ERVS_CheckDefect(threshold);
		m_i_checkdefect_ret = ret;

		str.Format(_T("%d"), ret);
		GetDlgItem(IDC_EDIT_RESULT_CHECK_DEFECT_RET)->SetWindowText(str);

	}

	//Get Result(Master) Image
	int len = 921600;		//640*480*3;
	ERVS_GetImage(GET_IMAGE_RESULT_FOUND_MASTER, 0, (char**)&m_found_image_object.data, &len);
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedCheckUseFindWithCheckdefect()
{
	// TODO: Add your control notification handler code here
	BOOL bCheckOptionDefect = IsDlgButtonChecked(IDC_CHECK_USE_FIND_WITH_CHECKDEFECT);

	if (bCheckOptionDefect)
	{
		ERVS_SetOptionFindWithCheckDefectOn();
	}
	else
	{
		ERVS_SetOptionFindWithCheckDefectOff();
	}

	//Get
	int ret = ERVS_GetOptionFindWithCheckDefect(); //ERVS_GetOptionPCA();

	if (ret > 0)
	{
		CheckDlgButton(IDC_CHECK_USE_FIND_WITH_CHECKDEFECT, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_USE_FIND_WITH_CHECKDEFECT, FALSE);
	}

	//-------------------------------------------------------------------------
	//Display on List Control
	CString str;
	if (ret > 0) 	str.Format(_T("Y"));
	else			str.Format(_T("N"));

	((CEyedeaExampleDlg *)GetParent())->m_list_information.SetItem(((CEyedeaExampleDlg *)GetParent())->m_select_list_number, 5, LVIF_TEXT, str, 0, 0, 0, NULL);
	//Display on List Control
	//-------------------------------------------------------------------------
}
#endif


void CEyedeaCheckDefectTabDlg::ThreadFunctionDraw()
{
	CRect rect_result_display;													//display rect
	GetDlgItem(IDC_STATIC_IMAGE_RESULT)->GetClientRect(&rect_result_display);			//get rect information on window
	CClientDC dc_result_display(GetDlgItem(IDC_STATIC_IMAGE_RESULT));					//device context for display mfc control

	CRect rect_histogram_display;													//display rect
	GetDlgItem(IDC_STATIC_HISTOGRAM)->GetClientRect(&rect_histogram_display);			//get rect information on window
	CClientDC dc_histogram_display(GetDlgItem(IDC_STATIC_HISTOGRAM));					//device context for display mfc control

	CRect rect_histogram_r_display;													//display rect
	GetDlgItem(IDC_STATIC_HISTOGRAM_R)->GetClientRect(&rect_histogram_r_display);			//get rect information on window
	CClientDC dc_histogram_r_display(GetDlgItem(IDC_STATIC_HISTOGRAM_R));					//device context for display mfc control

	CRect rect_histogram_g_display;													//display rect
	GetDlgItem(IDC_STATIC_HISTOGRAM_G)->GetClientRect(&rect_histogram_g_display);			//get rect information on window
	CClientDC dc_histogram_g_display(GetDlgItem(IDC_STATIC_HISTOGRAM_G));					//device context for display mfc control

	CRect rect_histogram_b_display;													//display rect
	GetDlgItem(IDC_STATIC_HISTOGRAM_B)->GetClientRect(&rect_histogram_b_display);			//get rect information on window
	CClientDC dc_histogram_b_display(GetDlgItem(IDC_STATIC_HISTOGRAM_B));					//device context for display mfc control

#if 0
	//cv::Mat found_image_object = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
	m_found_image_object = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
	CRect rect_display_found_object;													//display rect
	GetDlgItem(IDC_STATIC_IMAGE_FOUND_OBJECT)->GetClientRect(&rect_display_found_object);			//get rect information on window
	CClientDC dc_display_found_object(GetDlgItem(IDC_STATIC_IMAGE_FOUND_OBJECT));					//device context for display mfc
#endif

#if 0
	CRect rect_display_checkdefect_icon;													//display rect
	GetDlgItem(IDC_STATIC_IMAGE_FOUND_CHECKDEFECT_RET)->GetClientRect(&rect_display_checkdefect_icon);			//get rect information on window
	CClientDC dc_display_checkdefect_icon(GetDlgItem(IDC_STATIC_IMAGE_FOUND_CHECKDEFECT_RET));					//device context for display mfc
#endif

	CvvImage vImage;		//display class for display on MFC control

	while (m_run_thread)
	{
		if (m_b_draw_pause)
		{
			boost::this_thread::sleep(boost::posix_time::millisec(1));
			continue;
		}
		//---------------------------------------------------------------------------------
		//checkdefect display on static control
#if 0
		cv::Mat checkdefect = m_mat_warning;
		if (m_i_checkdefect_ret == 1)
		{
			checkdefect = m_mat_fail;
		}
		else if (m_i_checkdefect_ret == 2)
		{
			checkdefect = m_mat_pass;
		}

		vImage.CopyOf(&IplImage(checkdefect), 1);							//mat to vimage
		vImage.DrawToHDC(dc_display_checkdefect_icon.m_hDC, &rect_display_checkdefect_icon);				//draw on display_rect
#endif
		//checkdefect display on static control
		//---------------------------------------------------------------------------------

		//---------------------------------------------------------------------------------
		//base display on static control
		vImage.CopyOf(&IplImage(m_result_image), 1);							//mat to vimage
		vImage.DrawToHDC(dc_result_display.m_hDC, &rect_result_display);				//draw on display_rect

		DrawHistogram();

		//histogram
		vImage.CopyOf(&IplImage(m_result_histogram_image), 1);							//mat to vimage
		vImage.DrawToHDC(dc_histogram_display.m_hDC, &rect_histogram_display);				//draw on display_rect

		vImage.CopyOf(&IplImage(m_result_histogram_r_image), 1);							//mat to vimage
		vImage.DrawToHDC(dc_histogram_r_display.m_hDC, &rect_histogram_r_display);				//draw on display_rect

		vImage.CopyOf(&IplImage(m_result_histogram_g_image), 1);							//mat to vimage
		vImage.DrawToHDC(dc_histogram_g_display.m_hDC, &rect_histogram_g_display);				//draw on display_rect

		vImage.CopyOf(&IplImage(m_result_histogram_b_image), 1);							//mat to vimage
		vImage.DrawToHDC(dc_histogram_b_display.m_hDC, &rect_histogram_b_display);				//draw on display_rect

		//vImage.CopyOf(&IplImage(m_found_image_object), 1);							//mat to vimage
		//vImage.DrawToHDC(dc_display_found_object.m_hDC, &rect_display_found_object);				//draw on display_rect

		/*							
		//information
		float base_center_x = ERVS_GetBaseInfo(GET_BASE_CENTER_X);
		float base_center_y = ERVS_GetBaseInfo(GET_BASE_CENTER_Y);
		float base_degree = ERVS_GetBaseInfo(GET_BASE_DEGREE);
		float base_radian = ERVS_GetBaseInfo(GET_BASE_RADIAN);
		float base_value1 = ERVS_GetBaseInfo(GET_BASE_VALUE1);
		float base_value2 = ERVS_GetBaseInfo(GET_BASE_VALUE2);

		CString str;
		str.Format(_T("%.2f,%.2f"), base_center_x, base_center_y);
		GetDlgItem(IDC_EDIT_BASE_CENTER)->SetWindowText(str);
		str.Format(_T("%.2f˚(%.2f)"), base_degree, base_radian);
		GetDlgItem(IDC_EDIT_BASE_ANGLE)->SetWindowText(str);
		str.Format(_T("%.2f"), base_value1);
		GetDlgItem(IDC_EDIT_BASE_VALUE1)->SetWindowText(str);
		str.Format(_T("%.2f"), base_value2);
		GetDlgItem(IDC_EDIT_BASE_VALUE2)->SetWindowText(str);
		*/
		//base display on static control
		//---------------------------------------------------------------------------------
	};
}

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindObjectGet()
{
	// TODO: Add your control notification handler code here
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonVisionConfigFindObjectSet()
{
	// TODO: Add your control notification handler code here
}
#endif

/*
void CEyedeaCheckDefectTabDlg::OnBnClickedCheckFindMultiObjects()
{
	// TODO: Add your control notification handler code here
	BOOL bCheckOptionFindObjects = IsDlgButtonChecked(IDC_CHECK_FIND_MULTI_OBJECTS);

	if (bCheckOptionFindObjects)
	{
		ERVS_SetMode(ERVS_MODE_FIND_MULTIPLE);
	}
	else
	{
		ERVS_SetMode(ERVS_MODE_FIND_SINGLE);
	}

	//Get
	int ret = ERVS_GetMode(); //ERVS_GetOptionPCA();

	if (ret == ERVS_MODE_FIND_MULTIPLE)
	{
		CheckDlgButton(IDC_CHECK_FIND_MULTI_OBJECTS, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_FIND_MULTI_OBJECTS, FALSE);
	}

}
*/

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindGetInfo()
{
	// TODO: Add your control notification handler code here
	//DrawFindObectList();
#if 1
	while (m_ImgList.GetImageCount()) m_ImgList.Remove(0);

	//m_list_find_objects.DeleteAllItems();

	CString strIndex;
	GetDlgItem(IDC_EDIT_FIND_GET_INFO)->GetWindowText(strIndex);
	int user_index = _ttoi(strIndex);

	//GET DB
	//Get DB Count
	int nDB = ERVS_DB_Get_Count();
	int select_id = ERVS_DB_Get_Select_ID();

	int* p_id_range = NULL;
	if( nDB > 0 )
	{ 
		p_id_range = new int[nDB];

		for (int i = 0 ; i < nDB ; i++)
		{
			int id = ERVS_DB_Get_Info_Id(i);
			p_id_range[i] = id;
		}
	}

	int find_count = ERVS_GetFindObjectCount();
	int isCalibOK = ERVS_Calibration_isOK();

	CString unit; // = "pixel";
	unit.Format(_T("pixel"));
	CString strText;
	isCalibOK != 0 ? strText.Format(_T(" OK ")) : strText.Format(_T(" NO "));
	GetDlgItem(IDC_EDIT_CHECK_CAMERA_CALIBOK)->SetWindowText(strText);			

	CString str;

	for (int i = 0; i < find_count; i++)
	{
		float *p_camera_center_x = NULL;
		float *p_camera_center_y = NULL;
		float *p_robot_center_x = NULL;
		float *p_robot_center_y = NULL;
		float *p_angle = NULL;
		float *p_type = NULL;
		float *p_score = NULL;
		float find_index = -1;

		int nObject = ERVS_GetFindObjectInfo(i, p_id_range, nDB, &p_camera_center_x, &p_camera_center_y, &p_robot_center_x, &p_robot_center_y, &p_angle, &p_type, &p_score, &find_index);

		printf("find object count = %d\n", nObject);

		int len = 921600;		//640*480*3;
		cv::Mat find_object_image = cv::Mat(cv::Size(640, 480), CV_8UC3);

		if (find_index >= 0)
		{
			ERVS_GetFindObjectImage((int)find_index, (char**)&find_object_image.data, &len);
		}

		//cv::imshow("test", find_object_image);
		//cv::waitKey(0);

		cv::Mat image;
		cv::resize(find_object_image, image, cv::Size(160, 120));

		IplImage* iplimage_ = new IplImage(image);

		CBitmap *aCBitmap = IplImageToCBitmap(iplimage_);

		if (aCBitmap)
		{
			m_ImgList.Add(aCBitmap, RGB(0, 0, 0)); // 두번째 인자는 마스크로 검정색으로 해야 이미지 주변이 하얗게 나온다.
			CString str;

			str.Format(_T("- %d object -\n"), nObject);

			std::string std_str;

			for (int i = 0; i < nObject; i++)
			{
				CString str_temp;
				str_temp.Format(_T("[%d/%.2f] %.2f,%.2f,%d\n"), (int)p_type[i], p_score[i], p_camera_center_x[i], p_camera_center_y[i], (int)p_angle[i]);

				str += str_temp;
			}
			m_list_find_objects.InsertItem(i, str, i);

			delete aCBitmap;
			aCBitmap = NULL;
		}

		if (iplimage_)
		{
			delete iplimage_;
			iplimage_ = NULL;
		}

		if (p_camera_center_x != NULL) free(p_camera_center_x);
		if (p_camera_center_y != NULL) free(p_camera_center_y);
		if (p_robot_center_x != NULL) free(p_robot_center_x);
		if (p_robot_center_y != NULL) free(p_robot_center_y);
		if (p_angle != NULL) free(p_angle);
		if (p_type != NULL) free(p_type);
		if (p_score != NULL) free(p_score);
		//if (p_index != NULL) free(p_index);

	}

	if (p_id_range != NULL)
	{
		delete[] p_id_range;
		p_id_range = NULL;
	}
#endif
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindGetPose()
{
	// TODO: Add your control notification handler code here
	while (m_ImgList.GetImageCount()) m_ImgList.Remove(0);

	m_list_find_objects.DeleteAllItems();

	int find_count = ERVS_GetFindObjectCount();

	int isCalibOK = ERVS_Calibration_isOK();

	CString unit; // = "pixel";
	unit.Format(_T("pixel"));

	CString strText;
	isCalibOK != 0 ? strText.Format(_T(" OK ")) : strText.Format(_T(" NO "));
	GetDlgItem(IDC_EDIT_CHECK_CAMERA_CALIBOK)->SetWindowText(strText);			
	CString str;

	for (int i = 0; i < find_count; i++)
	{
		float camera_center_x = 0.0;
		float camera_center_y = 0.0;
		float robot_center_x = 0.0;
		float robot_center_y = 0.0;
		float angle = 0.0;

		ERVS_GetFindObjectPose(i, &camera_center_x, &camera_center_y, &robot_center_x, &robot_center_y, &angle);

		int len = 921600;		//640*480*3;
		cv::Mat find_object_image = cv::Mat(cv::Size(640, 480), CV_8UC3);
		ERVS_GetFindObjectImage(i, (char**)&find_object_image.data, &len);

		//cv::imshow("test", find_object_image);
		//cv::waitKey(0);

		cv::Mat image;
		cv::resize(find_object_image, image, cv::Size(160, 120));

		IplImage* iplimage_ = new IplImage(image);

		CBitmap *aCBitmap = IplImageToCBitmap(iplimage_);

		if (aCBitmap)
		{
			m_ImgList.Add(aCBitmap, RGB(0, 0, 0)); // 두번째 인자는 마스크로 검정색으로 해야 이미지 주변이 하얗게 나온다.
			CString str;
			str.Format(_T("%.2f , %.2f , %.2f"), camera_center_x, camera_center_y, angle);
			m_list_find_objects.InsertItem(i, str, i);

			delete aCBitmap;
			aCBitmap = NULL;
		}

		if (iplimage_)
		{
			delete iplimage_;
			iplimage_ = NULL;
		}
	}
}
#endif

void CEyedeaCheckDefectTabDlg::DrawFindObectList(void)
{
	// Remove every other image from the image list.
#if 0
	m_ImgList.Create(160, 120, ILC_COLOR24, 0, 0); //이미지 아이콘의 크기를 결정하는듯			

	for (int i = 0; i < m_ImgList.GetImageCount(); i++)
	{
		m_ImgList.Remove(i);
	}

	m_list_find_objects.DeleteAllItems();

	int nCalibrationInfo = ERVS_Calibration_GetCount();

	//cv::Mat calibration_image = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);
	cv::Mat calibration_image[2];
	calibration_image[0] = cv::imread("test.jpg");
	calibration_image[1] = cv::imread("test1.jpg");

	cv::Mat resize_image[2];
	cv::resize(calibration_image[0], resize_image[0], cv::Size(160, 120));
	cv::resize(calibration_image[1], resize_image[1], cv::Size(160, 120));

	IplImage* iplimage_1 = new IplImage(resize_image[0]);
	IplImage* iplimage_2 = new IplImage(resize_image[1]);

	CBitmap *aCBitmap1 = IplImageToCBitmap(iplimage_1);
	CBitmap *aCBitmap2 = IplImageToCBitmap(iplimage_2);

	m_ImgList.Add(aCBitmap1, RGB(0, 0, 0)); // 두번째 인자는 마스크로 검정색으로 해야 이미지 주변이 하얗게 나온다.
	m_ImgList.Add(aCBitmap2, RGB(0, 0, 0)); // 두번째 인자는 마스크로 검정색으로 해야 이미지 주변이 하얗게 나온다.

	m_list_find_objects.SetImageList(&m_ImgList, LVSIL_NORMAL);

	m_list_find_objects.InsertItem(0, _T("1"), 0);
	m_list_find_objects.InsertItem(1, _T("2"), 1);
#endif

#if 1
	//m_ImgList.Create(160, 120, ILC_COLOR24, 0, 0); //이미지 아이콘의 크기를 결정하는듯			
	//m_list_find_objects.SetImageList(&m_ImgList, LVSIL_NORMAL);

	//for (int i = 0; i < m_ImgList.GetImageCount(); i++)
	//{
	//	m_ImgList.Remove(i);
	//}
	//m_ImgList.
	//m_ImgList.Create(160, 120, ILC_COLOR24, 0, 0); //이미지 아이콘의 크기를 결정하는듯			
	//m_list_find_objects.SetImageList(&m_ImgList, LVSIL_NORMAL);
	m_list_find_objects.DeleteAllItems();

	cv::Mat calibration_image[2];
	calibration_image[0] = cv::imread("test.jpg");
	calibration_image[1] = cv::imread("test1.jpg");

	for (int i = 0; i < 2; i++)
	{
		cv::Mat image;
		cv::resize(calibration_image[i], image, cv::Size(160, 120));

		IplImage* iplimage_ = new IplImage(image);

		//cvShowImage("tt", iplimage_);
		//cv::waitKey(0);

		//CBitmap *aCBitmap = IplImageToCBitmap(image2);
		CBitmap *aCBitmap = IplImageToCBitmap(iplimage_);
		//CBitmap *aCBitmap = IplImageToCBitmap((IplImage*)&image);

		//
		if (aCBitmap)
		{
			m_ImgList.Add(aCBitmap, RGB(0, 0, 0)); // 두번째 인자는 마스크로 검정색으로 해야 이미지 주변이 하얗게 나온다.
			CString str;
			str.Format(_T("%.2f,%.2f"), 100.123, 123.100);
			m_list_find_objects.InsertItem(i, str, i);

			delete aCBitmap;
			aCBitmap = NULL;
		}

		if (iplimage_)
		{
			delete iplimage_;
			iplimage_ = NULL;
		}
	}

	//m_ImgList.DeleteImageList();
#endif
}

CBitmap* CEyedeaCheckDefectTabDlg::IplImageToCBitmap(IplImage* img)
{
	//CDC dc;
	CDC memDC;

	//if (!dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL))
	//dc.GetDC();
	CDC* pDC = this->GetDC();

	/*
	//if (!dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL))
	if (!pDC->CreateDC(_T("DISPLAY"), NULL, NULL, NULL))
	{
		printf("fail. createDC\n");
		return NULL;
	}
	*/

	//if (!memDC.CreateCompatibleDC(&dc))
	if (!memDC.CreateCompatibleDC(pDC))
	{
		printf("fail. CreateCompatibleDC\n");
		return NULL;
	}


	CBitmap* bmp = new CBitmap();
	CBitmap* pOldBitmap;

	//bmp->CreateCompatibleBitmap(&dc, img->width, img->height);
	bmp->CreateCompatibleBitmap(pDC, img->width, img->height);
	pOldBitmap = memDC.SelectObject(bmp);

	CvvImage cvImage; // you will need OpenCV_2.2.0- to use CvvImage 
	cvImage.CopyOf(img);
	cvImage.Show(memDC.m_hDC, 0, 0, img->width, img->height, 0, 0);
	cvImage.Destroy();

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	//dc.DeleteDC();

	return bmp;
}

void CEyedeaCheckDefectTabDlg::RoadAllFromERVS()
{
	//OnBnClickedCheckUsePca();

	//OnBnClickedCheckHistogram();
	//check
	float use_histogram = ERVS_GetVisionConfigOption(VISION_CONFIG_USE_HISTOGRAM);

	if (use_histogram)
	{
		CheckDlgButton(IDC_CHECK_HISTOGRAM, TRUE);
}
	else
	{
		CheckDlgButton(IDC_CHECK_HISTOGRAM, FALSE);
	}


	
}

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindObjectClassifierThreGet()
{
	// TODO: Add your control notification handler code here
}
#endif

void CEyedeaCheckDefectTabDlg::OnBnClickedButtonFindGetInfo2()
{
	// TODO: Add your control notification handler code here
	//while (m_ImgList.GetImageCount()) m_ImgList.Remove(0);
	//m_list_result_index.ResetContent();

	cv::RNG rng(0xFFFFFFFF);
	unsigned int run_count = 0;

	//while (1)
	{
#if 0
		ERVS_Disconnect();

		int sleep_time = rng.uniform(1, 1000);
		Sleep(sleep_time);

		printf("%d : sleep(%d)\n", run_count++, sleep_time);

		char ip_addr[255];
		sprintf(ip_addr, "%d.%d.%d.%d", (int)192, (int)168, (int)100, (int)3);
		int ret = ERVS_Connect(ip_addr, 4000);
#endif

		m_tree_result.DeleteAllItems();

		CString strIndex;
		GetDlgItem(IDC_EDIT_FIND_GET_INFO)->GetWindowText(strIndex);
		int user_index = _ttoi(strIndex);

		//Get Select DB Info from Parent List Control
		int list_num_ui = ((CEyedeaExampleDlg *)GetParent())->m_list_information.GetItemCount();
		POSITION pos = ((CEyedeaExampleDlg *)GetParent())->m_list_information.GetFirstSelectedItemPosition();

		//Get DB Count
		int nDB = ERVS_DB_Get_Count();
		int select_id = ERVS_DB_Get_Select_ID();

		int* p_id_range = NULL;
		int id_index = 0;
		if (nDB > 0)
		{
			p_id_range = new int[nDB];

			for (int nItem = 0; nItem < ((CEyedeaExampleDlg *)GetParent())->m_list_information.GetItemCount(); nItem++)
			{
				int unState = ((CEyedeaExampleDlg *)GetParent())->m_list_information.GetItemState(nItem, LVIS_STATEIMAGEMASK);

				if (unState == 0x2000)
				{
					//id check
					CString list_id;
					list_id = ((CEyedeaExampleDlg *)GetParent())->m_list_information.GetItemText(nItem, 1/*id*/);

					int i_list_id = _ttoi(list_id);

					p_id_range[id_index++] = i_list_id;
				}
			}
		}

		//for (int i = 0; i < 10000; i++)
		{
			float *p_id = NULL;
			float *p_camera_center_x = NULL;
			float *p_camera_center_y = NULL;
			float *p_robot_center_x = NULL;
			float *p_robot_center_y = NULL;

			float *p_camera_bound_center_x = NULL;
			float *p_camera_bound_center_y = NULL;
			float *p_robot_bound_center_x = NULL;
			float *p_robot_bound_center_y = NULL;

			float *p_camera_mass_center_x = NULL;
			float *p_camera_mass_center_y = NULL;
			float *p_robot_mass_center_x = NULL;
			float *p_robot_mass_center_y = NULL;

			float *p_circle_rx = NULL;
			float *p_circle_ry = NULL;

			float *p_line1_x = NULL;
			float *p_line1_y = NULL;
			float *p_line2_x = NULL;
			float *p_line2_y = NULL;

			float *p_angle = NULL;
			float *p_type = NULL;
			float *p_score = NULL;

			//printf("test = %d\n", i);
			CString str;
			GetDlgItem(IDC_EDIT_FIND_MAX_COUNT)->GetWindowText(str);
			int nMaxObjects = _ttoi(str);

			int nObject = 0;
			BOOL bCheckOptionUsePrevImage = IsDlgButtonChecked(IDC_CHECK_GETINFO_USE_PREV_IMAGE);

			if (bCheckOptionUsePrevImage)
			{
				nObject = ERVS_DetectWithPrevImage(user_index, nMaxObjects,
					&p_id,
					&p_camera_center_x, &p_camera_center_y, &p_robot_center_x, &p_robot_center_y,
					&p_camera_bound_center_x, &p_camera_bound_center_y, &p_robot_bound_center_x, &p_robot_bound_center_y,
					&p_camera_mass_center_x, &p_camera_mass_center_y, &p_robot_mass_center_x, &p_robot_mass_center_y,
					&p_circle_rx, &p_circle_ry,
					&p_line1_x, &p_line1_y, &p_line2_x, &p_line2_y,
					&p_angle,
					&p_type,
					&p_score);
			}
			else
			{
				nObject = ERVS_DetectWithGrab(user_index, nMaxObjects,
					&p_id,
					&p_camera_center_x, &p_camera_center_y, &p_robot_center_x, &p_robot_center_y,
					&p_camera_bound_center_x, &p_camera_bound_center_y, &p_robot_bound_center_x, &p_robot_bound_center_y,
					&p_camera_mass_center_x, &p_camera_mass_center_y, &p_robot_mass_center_x, &p_robot_mass_center_y,
					&p_circle_rx, &p_circle_ry,
					&p_line1_x, &p_line1_y, &p_line2_x, &p_line2_y,
					&p_angle,
					&p_type,
					&p_score);
			}

			time_t curr_time;
			struct tm *curr_tm;
			curr_time = time(NULL);
			curr_tm = localtime(&curr_time);

			//CString str;
			str.Format(_T("%d-%d-%d-%d-%d-%d : find object count = %d\r\n"), curr_tm->tm_year + 1900, curr_tm->tm_mon + 1, curr_tm->tm_mday, curr_tm->tm_hour, curr_tm->tm_min, curr_tm->tm_sec, nObject);
			//printf("%s", str);
			MyTextOut(str, RGB(0, 0, 255));

			int index = 0;
			int sub_index = 0;

			HTREEITEM  hRoot = NULL;

			for (int i = 0; i < nObject; i++)
			{
				//printf("%s", str);
				if ((int)p_id[i] % 1000 == 0)
				{
					str.Format(_T(" - [%d] : id=%04d/type=%d/cpos=(%.2f,%.2f)/rpos=(%.2f,%.2f)/cbpos=(%.2f,%.2f)/rbpos=(%.2f,%.2f)/cmpos=(%.2f,%.2f)/rmpos=(%.2f,%.2f)/angle=%d/score=%.2f\n"),
						index, (int)p_id[i], (int)p_type[i],
						p_camera_center_x[i], p_camera_center_y[i], p_robot_center_x[i], p_robot_center_y[i],
						p_camera_bound_center_x[i], p_camera_bound_center_y[i], p_robot_bound_center_x[i], p_robot_bound_center_y[i],
						p_camera_mass_center_x[i], p_camera_mass_center_y[i], p_robot_mass_center_x[i], p_robot_mass_center_y[i],
						(int)p_angle[i], p_score[i]);

					MyTextOut(str, RGB(255, 100, 0));

					CString str_tree;
					str_tree.Format(_T("ID(%04d)-%d"), (int)p_id[i], index);

					hRoot = m_tree_result.InsertItem(str_tree, 0/* nImage */, 1/* nSelectedImage */, TVI_ROOT, TVI_LAST);
					m_tree_result.Expand(hRoot, TVE_EXPAND);

					//HTREEITEM  hChild;
					//hChild = m_tree_result.InsertItem(L"image", 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST);
					index++;
					sub_index = 0;
				}
				else
				{
					if (p_type[i] == -200)	//circle
					{
						str.Format(_T("       : (CIRCLE) id=%04d/type=%d/cpos=(%.2f,%.2f)/rpos=(%.2f,%.2f)/radiusx=(%.2f)/radiusy=(%.2f)/score=%.2f\n"),
							(int)p_id[i], (int)p_type[i],
							p_camera_center_x[i], p_camera_center_y[i], p_robot_center_x[i], p_robot_center_y[i],
							p_circle_rx[i], p_circle_ry[i],
							p_score[i]);
					}
					else if (p_type[i] == -300)	//line
					{
						str.Format(_T("       : (LINE) id=%04d/type=%d/cpos=(%.2f,%.2f)/rpos=(%.2f,%.2f)/line1=(%.2f,%.2f)/line2=(%.2f,%.2f)/angle=%d/score=%.2f\n"),
							(int)p_id[i], (int)p_type[i],
							p_camera_center_x[i], p_camera_center_y[i], p_robot_center_x[i], p_robot_center_y[i],
							p_line1_x[i], p_line1_y[i], p_line2_x[i], p_line2_y[i],
							(int)p_angle[i], p_score[i]);
					}
					else
					{
						str.Format(_T("       : (REGION) id=%04d/type=%d/cpos=(%.2f,%.2f)/rpos=(%.2f,%.2f)/cbpos=(%.2f,%.2f)/rbpos=(%.2f,%.2f)/cmpos=(%.2f,%.2f)/rmpos=(%.2f,%.2f)/angle=%d/score=%.2f\n"),
							(int)p_id[i], (int)p_type[i],
							p_camera_center_x[i], p_camera_center_y[i], p_robot_center_x[i], p_robot_center_y[i],
							p_camera_bound_center_x[i], p_camera_bound_center_y[i], p_robot_bound_center_x[i], p_robot_bound_center_y[i],
							p_camera_mass_center_x[i], p_camera_mass_center_y[i], p_robot_mass_center_x[i], p_robot_mass_center_y[i],
							(int)p_angle[i], p_score[i]);
					}

					MyTextOut(str, RGB(0, 100, 255));

					if (hRoot != NULL)
					{
						CString str_tree;
						str_tree.Format(_T("ID(%04d)-%d"), (int)p_id[i], sub_index);

						HTREEITEM  hChild;
						hChild = m_tree_result.InsertItem(str_tree, 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST);

						m_tree_result.Expand(hRoot, TVE_EXPAND);
						m_tree_result.Expand(hChild, TVE_EXPAND);
					}

					sub_index++;
				}
				//MyTextOut(" - [%d] : id=%04d / type=%d / cpos=(%.4f, %.4f) / rpos=(%.4f, %.4f) / angle=%d / score=%.4f\n", i, (int)p_id[i], (int)p_type[i], p_camera_center_x[i], p_camera_center_y[i], p_robot_center_x[i], p_robot_center_y[i], p_angle[i], p_score[i]);
			}

			if (p_camera_center_x != NULL) free(p_camera_center_x);
			if (p_camera_center_y != NULL) free(p_camera_center_y);
			if (p_robot_center_x != NULL) free(p_robot_center_x);
			if (p_robot_center_y != NULL) free(p_robot_center_y);
			if (p_camera_bound_center_x != NULL) free(p_camera_bound_center_x);
			if (p_camera_bound_center_y != NULL) free(p_camera_bound_center_y);
			if (p_robot_bound_center_x != NULL) free(p_robot_bound_center_x);
			if (p_robot_bound_center_y != NULL) free(p_robot_bound_center_y);
			if (p_camera_mass_center_x != NULL) free(p_camera_mass_center_x);
			if (p_camera_mass_center_y != NULL) free(p_camera_mass_center_y);
			if (p_robot_mass_center_x != NULL) free(p_robot_mass_center_x);
			if (p_robot_mass_center_y != NULL) free(p_robot_mass_center_y);
			if (p_angle != NULL) free(p_angle);
			if (p_type != NULL) free(p_type);
			if (p_score != NULL) free(p_score);

			//Result image
			int len = 921600;
			//ERVS_GetImage(GET_IMAGE_RESULT, -1, (char**)&m_result_image.data, &len);
			ERVS_GetFindObjectResultImage(-1, -1, (char**)&m_result_image.data, &len);

			Sleep(1);
		}

#if 0
		int len = 921600;		//640*480*3;
		cv::Mat find_object_image = cv::Mat(cv::Size(640, 480), CV_8UC3);

		if (find_index >= 0)
		{
			ERVS_GetFindObjectImage((int)find_index, (char**)&find_object_image.data, &len);
		}

		//cv::imshow("test", find_object_image);
		//cv::waitKey(0);

		cv::Mat image;
		cv::resize(find_object_image, image, cv::Size(160, 120));

		IplImage* iplimage_ = new IplImage(image);

		CBitmap *aCBitmap = IplImageToCBitmap(iplimage_);

		if (aCBitmap)
		{
			m_ImgList.Add(aCBitmap, RGB(0, 0, 0)); // 두번째 인자는 마스크로 검정색으로 해야 이미지 주변이 하얗게 나온다.
			CString str;

			str.Format(_T("- %d object -\n"), nObject);

			std::string std_str;

			for (int i = 0; i < nObject; i++)
			{
				CString str_temp;
				str_temp.Format(_T("[%d/%.2f] %.2f,%.2f,%d\n"), (int)p_type[i], p_score[i], p_camera_center_x[i], p_camera_center_y[i], (int)p_angle[i]);

				str += str_temp;
			}

			//m_list_find_objects.InsertItem(i, str, i);
			m_list_find_objects.InsertItem(0, str, 0);

			delete aCBitmap;
			aCBitmap = NULL;
		}

		if (iplimage_)
		{
			delete iplimage_;
			iplimage_ = NULL;
		}
#endif

		if (p_id_range != NULL)
		{
			delete[] p_id_range;
			p_id_range = NULL;
		}
	}
}

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedCheckDependencySearcharea()
{
	// TODO: Add your control notification handler code here
	BOOL bCheckOptionDependencySearcharea = IsDlgButtonChecked(IDC_CHECK_DEPENDENCY_SEARCHAREA);

	if (bCheckOptionDependencySearcharea)
	{
		ERVS_SetVisionConfigOption(SEARCHAREA_VISION_CONFIG_ONLY_ONE_OBJECT, 1);
	}
	else
	{
		ERVS_SetVisionConfigOption(SEARCHAREA_VISION_CONFIG_ONLY_ONE_OBJECT, 0);
	}

	//check
	float only_one_option = ERVS_GetVisionConfigOption(SEARCHAREA_VISION_CONFIG_ONLY_ONE_OBJECT);

	if (only_one_option)
	{
		CheckDlgButton(IDC_CHECK_DEPENDENCY_SEARCHAREA, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_DEPENDENCY_SEARCHAREA, FALSE);
	}
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonSetZoomImage()
{
	// TODO: Add your control notification handler code here
	//USER_COMMAND_SET_ZOOM_IMAGE
	//m_combo_get_image_option_base.SetCurSel(1);		//Get Raw Image

	m_command = USER_COMMAND_SET_ZOOM_IMAGE;
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonResetZoomImage()
{
	// TODO: Add your control notification handler code here
	ERVS_ResetZoomArea();
}
#endif

void CEyedeaCheckDefectTabDlg::ManageList()
{
#if 0 
	//check dependency list
	//m_combo_dependency_list
	//Get DB Count and List
	std::vector<int> vec_base_id_list;
	int nDB = ERVS_DB_Get_Count();
	//CString str;
	for (int nItem = 0; nItem < nDB; nItem++)
	{
		int id = ERVS_DB_Get_Info_Id(nItem);

		if (id % 1000 == 0)
		{
			vec_base_id_list.push_back(id);
			//str.Format(_T("%04d"), id);
			//m_combo_dependency_list.AddString(str);
		}
	}

	int nList = m_combo_dependency_list.GetCount();
	int nSel = m_combo_dependency_list.GetCurSel();

	bool find = false;
	int find_sel_num = -1;
	if (nSel > 0)
	{
		CString selectDropListString;
		m_combo_dependency_list.GetLBText(m_combo_dependency_list.GetCurSel(), selectDropListString);

		int list_id = _ttoi(selectDropListString);

		//find
		for (int i = 0; i < vec_base_id_list.size(); i++)
		{
			if (vec_base_id_list[i] == list_id)
			{
				find = true;
				find_sel_num = m_combo_dependency_list.GetCurSel();
				break;
			}
		}


	}

	for (int i = 0; i < nList; i++)
	{
		CString selectDropListString;
		m_combo_dependency_list.GetLBText(m_combo_dependency_list.GetCurSel(), selectDropListString);

		int list_id = _ttoi(selectDropListString);

		//find
		bool find = false;
		int find_sel_num = -1;

		for (int i = 0; i < vec_base_id_list.size(); i++)
		{
			if (vec_base_id_list[i] == list_id)
			{
				find = true;
				find_sel_num = m_combo_dependency_list.GetCurSel();
				break;
			}
		}

		if (find == false)
		{

		}
	}
#endif
}

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedButtonDepIdCheck()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_DEP_ID)->GetWindowText(str);
	int dep_id = _ttoi(str);

	int nDB = ERVS_DB_Get_Count();
	//CString str;
	bool find = false;

	for (int nItem = 0; nItem < nDB; nItem++)
	{
		int id = ERVS_DB_Get_Info_Id(nItem);

		if (id % 1000 == 0)
		{
			if (dep_id == id)
			{
				find = true;
				break;
			}
		}
	}

	if (find)
	{
		CheckDlgButton(IDC_CHECK_DEPENDENCY_SEARCHAREA, TRUE);
	}

	OnBnClickedCheckDependencySearcharea();
}
#endif

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedCheckUseCalibrationImage()
{
	BOOL bCheckOptionUseCalibrationImage = IsDlgButtonChecked(IDC_CHECK_USE_CALIBRATION_IMAGE);

	if (bCheckOptionUseCalibrationImage)
	{
		ERVS_SetVisionConfigOption(VISION_CONFIG_USE_CALIBRATION_IMAGE, 1);
	}
	else
	{
		ERVS_SetVisionConfigOption(VISION_CONFIG_USE_CALIBRATION_IMAGE, 0);
	}

	//check
	float use_calibration_image = ERVS_GetVisionConfigOption(VISION_CONFIG_USE_CALIBRATION_IMAGE);

	if (use_calibration_image)
	{
		CheckDlgButton(IDC_CHECK_USE_CALIBRATION_IMAGE, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_USE_CALIBRATION_IMAGE, FALSE);
	}
}
#endif

void CEyedeaCheckDefectTabDlg::OnBnClickedCheckHistogram()
{
	// TODO: Add your control notification handler code here
	BOOL bCheckOptionUseHistogram = IsDlgButtonChecked(IDC_CHECK_HISTOGRAM);

	if (bCheckOptionUseHistogram)
	{
		ERVS_SetVisionConfigOption(VISION_CONFIG_USE_HISTOGRAM, 1);
	}
	else
	{
		ERVS_SetVisionConfigOption(VISION_CONFIG_USE_HISTOGRAM, 0);
	}

	//check
	float use_histogram = ERVS_GetVisionConfigOption(VISION_CONFIG_USE_HISTOGRAM);

	if (use_histogram)
	{
		CheckDlgButton(IDC_CHECK_HISTOGRAM, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_HISTOGRAM, FALSE);
	}
}

#if 0
void CEyedeaCheckDefectTabDlg::OnLbnDblclkListResult()
{
	// TODO: Add your control notification handler code here
	int nList = m_list_result_index.GetCurSel();       //인자 얻기

	//Get Result Information from ERVS
	//Result image
	int len = 921600;
	ERVS_GetImage(GET_IMAGE_RESULT, nList, (char**)&m_result_image.data, &len);

}
#endif

void CEyedeaCheckDefectTabDlg::OnNMDblclkTreeResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	// TODO: Add your control notification handler code here
	//CTreeCtrl* tree = &GetTreeCtrl(); // 트리뷰 일 경우

	HTREEITEM hItem;
	CString strName;

	hItem = m_tree_result.GetNextItem(NULL, TVGN_CARET); // 현재 선택된 아이템의 핸들을 가져온다.
	strName = m_tree_result.GetItemText(hItem); // 그 아이템의 이름을 얻어온다.
	//MessageBox(strName);

	//m_tree_result.Expand(hItem, TVE_EXPAND);

	CT2CA pszConvertedAnsiString(strName);
	std::string str_cur_items(pszConvertedAnsiString);


	//me
	int me_index = -1;
	size_t pos;
	if( (pos = str_cur_items.find("-")) != std::string::npos)
	{
		// "world!"라는 문자열을 찾았을 때의 동작 
		std::string str_me_index = str_cur_items.substr(pos+1, str_cur_items.length());

		me_index = atoi(str_me_index.c_str());
		//printf("me index = %s\n", str_me_index.c_str());
	}

	hItem = m_tree_result.GetNextItem(hItem, TVGN_PARENT); // 현재 선택되어진 아이템의 상위 아이템을 가져온다.
	strName = m_tree_result.GetItemText(hItem); // 그 아이템의 이름을 얻어온다.
	//MessageBox(strName);

	//m_tree_result.Expand(hItem, TVE_EXPAND);

	CT2CA pszConvertedAnsiString2(strName);
	std::string str_mom_items(pszConvertedAnsiString2);

	//mom
	int mom_index = -1;
	if ((pos = str_mom_items.find("-")) != std::string::npos)
	{
		std::string str_mom_index = str_mom_items.substr(pos + 1, str_mom_items.length());

		//printf("me index = %s\n", str_me_index.c_str());
		mom_index = atoi(str_mom_index.c_str());
	}

	int index = -1;
	if (me_index >= 0)
	{
		if (mom_index < 0)
		{
			mom_index = me_index;
			me_index = -1;
		}

		//Get Information from ERVS
		float id = 0.0;
		float camera_center_x = 0.0;
		float camera_center_y = 0.0;
		float robot_center_x = 0.0;
		float robot_center_y = 0.0;
		float angle = 0.0;
		float type = 0.0;
		float score = 0.0;

		m_histogram_size = 0;
		if (m_p_histogram != NULL)
		{
			free(m_p_histogram);
			m_p_histogram = NULL;
		}

		if (m_p_histogram_r != NULL)
		{
			free(m_p_histogram_r);
			m_p_histogram_r = NULL;
		}

		if (m_p_histogram_g != NULL)
		{
			free(m_p_histogram_g);
			m_p_histogram_g = NULL;
		}

		if (m_p_histogram_b != NULL)
		{
			free(m_p_histogram_b);
			m_p_histogram_b = NULL;
		}

		ERVS_GetFindObjectResultInfo(mom_index, me_index, &id, &camera_center_x, &camera_center_y, &robot_center_x, &robot_center_y, &angle, &type, &score, &m_p_histogram, &m_p_histogram_r, &m_p_histogram_g, &m_p_histogram_b, &m_histogram_size);
		
		m_select_id = (int)id;

		/*
		CString str;
		str.Format(_T("camera(%.2f, %.2f), robot(%.2f, %.2f), angle(%.2f)"), camera_center_x, camera_center_y, robot_center_x, robot_center_y, angle);
		GetDlgItem(IDC_EDIT_RESULT_POSITION)->SetWindowText(str);

		str.Format(_T("edge=%.2f"), score);
		GetDlgItem(IDC_EDIT_RESULT_EDGE_MATCHING_SCORE)->SetWindowText(str);
		*/

		printf("histogram size = %f\n", m_histogram_size);

		//Get Result Information from ERVS
		//Result image
		int len = 921600;
		ERVS_GetFindObjectResultImage(mom_index, me_index, (char**)&m_result_image.data, &len);

		OnBnClickedButtonHistoGet();
		OnBnClickedButtonHistoGet2();
		OnBnClickedButtonHistoGet3();
		OnBnClickedButtonHistoGet4();
	}
	
	*pResult = 1;
}

void CEyedeaCheckDefectTabDlg::DrawHistogram(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	int histogram_size = (int)m_histogram_size;
	if (histogram_size > 0)
	{
		m_result_histogram_image = 0;
		m_result_histogram_r_image = 0;
		m_result_histogram_g_image = 0;
		m_result_histogram_b_image = 0;

		if (m_result_histogram_image.cols < histogram_size)
		{
			m_result_histogram_image = cv::Mat::zeros(cv::Size(histogram_size, 255), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
		}

		if (m_result_histogram_r_image.cols < histogram_size)
		{
			m_result_histogram_r_image = cv::Mat::zeros(cv::Size(histogram_size, 255), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
		}

		if (m_result_histogram_g_image.cols < histogram_size)
		{
			m_result_histogram_g_image = cv::Mat::zeros(cv::Size(histogram_size, 255), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
		}

		if (m_result_histogram_b_image.cols < histogram_size)
		{
			m_result_histogram_b_image = cv::Mat::zeros(cv::Size(histogram_size, 255), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
		}

		int start_x_v = (m_result_histogram_image.cols - histogram_size) / 2;
		int start_x_r = (m_result_histogram_r_image.cols - histogram_size) / 2;
		int start_x_g = (m_result_histogram_g_image.cols - histogram_size) / 2;
		int start_x_b = (m_result_histogram_b_image.cols - histogram_size) / 2;

		//scale 
		CRect rect_histogram_gray;
		GetDlgItem(IDC_STATIC_HISTOGRAM)->GetWindowRect(&rect_histogram_gray);
		ScreenToClient(&rect_histogram_gray);

		CRect rect_histogram_red;
		GetDlgItem(IDC_STATIC_HISTOGRAM_R)->GetWindowRect(&rect_histogram_red);
		ScreenToClient(&rect_histogram_red);

		CRect rect_histogram_green;
		GetDlgItem(IDC_STATIC_HISTOGRAM_G)->GetWindowRect(&rect_histogram_green);
		ScreenToClient(&rect_histogram_green);

		CRect rect_histogram_blue;
		GetDlgItem(IDC_STATIC_HISTOGRAM_B)->GetWindowRect(&rect_histogram_blue);
		ScreenToClient(&rect_histogram_blue);

		m_f_histogram_w_scale_v = (float)rect_histogram_gray.Width() / (float)m_result_histogram_image.cols;
		m_f_histogram_w_scale_r = (float)rect_histogram_red.Width() / (float)m_result_histogram_r_image.cols;
		m_f_histogram_w_scale_g = (float)rect_histogram_green.Width() / (float)m_result_histogram_g_image.cols;
		m_f_histogram_w_scale_b = (float)rect_histogram_blue.Width() / (float)m_result_histogram_b_image.cols;

		m_f_histogram_size_v_on_ui = histogram_size * m_f_histogram_w_scale_v;
		m_f_histogram_size_r_on_ui = histogram_size * m_f_histogram_w_scale_r;
		m_f_histogram_size_g_on_ui = histogram_size * m_f_histogram_w_scale_g;
		m_f_histogram_size_b_on_ui = histogram_size * m_f_histogram_w_scale_b;

		m_f_start_x_v_on_ui = (float)start_x_v * m_f_histogram_w_scale_v;
		m_f_start_x_r_on_ui = (float)start_x_r * m_f_histogram_w_scale_r;
		m_f_start_x_g_on_ui = (float)start_x_g * m_f_histogram_w_scale_g;
		m_f_start_x_b_on_ui = (float)start_x_b * m_f_histogram_w_scale_b;

		//float scale_x = 
		//m_f_histogram_size_on_ui
		//m_f_start_x_v_on_ui
		//m_f_start_x_r_on_ui
		//m_f_start_x_g_on_ui
		//m_f_start_x_b_on_ui

		//guide line
		cv::Scalar guide_rect_fill_colof = cv::Scalar(50, 50, 50);
		cv::Scalar guide_line_colof = cv::Scalar(128, 128, 128);

		//gray
		cv::rectangle(m_result_histogram_image, cv::Rect(start_x_v, 0, histogram_size, m_result_histogram_image.rows), guide_rect_fill_colof, CV_FILLED);
		cv::line(m_result_histogram_image, cv::Point(start_x_v, m_result_histogram_image.rows), cv::Point(start_x_v, 0), guide_line_colof, 1);
		cv::line(m_result_histogram_image, cv::Point(start_x_v + histogram_size, m_result_histogram_image.rows), cv::Point(start_x_v + histogram_size, 0), guide_line_colof, 1);
		//r
		cv::rectangle(m_result_histogram_r_image, cv::Rect(start_x_r, 0, histogram_size, m_result_histogram_r_image.rows), guide_rect_fill_colof, CV_FILLED);
		cv::line(m_result_histogram_r_image, cv::Point(start_x_r, m_result_histogram_r_image.rows), cv::Point(start_x_r, 0), guide_line_colof, 1);
		cv::line(m_result_histogram_r_image, cv::Point(start_x_r + histogram_size, m_result_histogram_r_image.rows), cv::Point(start_x_r + histogram_size, 0), guide_line_colof, 1);
		//g
		cv::rectangle(m_result_histogram_g_image, cv::Rect(start_x_g, 0, histogram_size, m_result_histogram_g_image.rows), guide_rect_fill_colof, CV_FILLED);
		cv::line(m_result_histogram_g_image, cv::Point(start_x_g, m_result_histogram_g_image.rows), cv::Point(start_x_g, 0), guide_line_colof, 1);
		cv::line(m_result_histogram_g_image, cv::Point(start_x_g + histogram_size, m_result_histogram_g_image.rows), cv::Point(start_x_g + histogram_size, 0), guide_line_colof, 1);
		//b
		cv::rectangle(m_result_histogram_b_image, cv::Rect(start_x_b, 0, histogram_size, m_result_histogram_b_image.rows), guide_rect_fill_colof, CV_FILLED);
		cv::line(m_result_histogram_b_image, cv::Point(start_x_b, m_result_histogram_b_image.rows), cv::Point(start_x_b, 0), guide_line_colof, 1);
		cv::line(m_result_histogram_b_image, cv::Point(start_x_b + histogram_size, m_result_histogram_b_image.rows), cv::Point(start_x_b + histogram_size, 0), guide_line_colof, 1);

		//Draw Range
		int min_value = 0;
		int max_value = 0;
		ERVS_Histogram_Get_Range(m_select_id, 0, &min_value, &max_value);

		cv::rectangle(m_result_histogram_image, cv::Rect(start_x_v+ min_value, 0, max_value - min_value, m_result_histogram_image.rows), cv::Scalar(128,128,128), CV_FILLED);
		//cv::line(m_result_histogram_image, cv::Point(start_x_v + m_i_histogmra_gray_min, m_result_histogram_image.rows), cv::Point(start_x_v + m_i_histogmra_gray_min, 0), cv::Scalar(128, 128, 128), 2);
		//cv::line(m_result_histogram_image, cv::Point(start_x_v + m_i_histogmra_gray_max, m_result_histogram_image.rows), cv::Point(start_x_v + m_i_histogmra_gray_max, 0), cv::Scalar(128, 128, 128), 2);

		min_value = 0;
		max_value = 0;
		ERVS_Histogram_Get_Range(m_select_id, 1, &min_value, &max_value);

		cv::rectangle(m_result_histogram_r_image, cv::Rect(start_x_r + min_value, 0, max_value - min_value, m_result_histogram_r_image.rows), cv::Scalar(128, 128, 128), CV_FILLED);
		//cv::line(m_result_histogram_r_image, cv::Point(start_x_r + m_i_histogmra_red_min, m_result_histogram_r_image.rows), cv::Point(start_x_r + m_i_histogmra_red_min, 0), cv::Scalar(0, 0, 255), 2);
		//cv::line(m_result_histogram_r_image, cv::Point(start_x_r + m_i_histogmra_red_max, m_result_histogram_r_image.rows), cv::Point(start_x_r + m_i_histogmra_red_max, 0), cv::Scalar(0, 0, 255), 2);

		min_value = 0;
		max_value = 0;
		ERVS_Histogram_Get_Range(m_select_id, 2, &min_value, &max_value);

		cv::rectangle(m_result_histogram_g_image, cv::Rect(start_x_g + min_value, 0, max_value - min_value, m_result_histogram_g_image.rows), cv::Scalar(128, 128, 128), CV_FILLED);
		//cv::line(m_result_histogram_g_image, cv::Point(start_x_g + m_i_histogmra_green_min, m_result_histogram_g_image.rows), cv::Point(start_x_g + m_i_histogmra_green_min, 0), cv::Scalar(0, 255, 0), 2);
		//cv::line(m_result_histogram_g_image, cv::Point(start_x_g + m_i_histogmra_green_max, m_result_histogram_g_image.rows), cv::Point(start_x_g + m_i_histogmra_green_max, 0), cv::Scalar(0, 255, 0), 2);

		min_value = 0;
		max_value = 0;
		ERVS_Histogram_Get_Range(m_select_id, 3, &min_value, &max_value);

		cv::rectangle(m_result_histogram_b_image, cv::Rect(start_x_b + min_value, 0, max_value - min_value, m_result_histogram_b_image.rows), cv::Scalar(128, 128, 128), CV_FILLED);
		//cv::line(m_result_histogram_b_image, cv::Point(start_x_b + m_i_histogmra_blue_min, m_result_histogram_b_image.rows), cv::Point(start_x_b + m_i_histogmra_blue_min, 0), cv::Scalar(255, 0, 0), 2);
		//cv::line(m_result_histogram_b_image, cv::Point(start_x_b + m_i_histogmra_blue_max, m_result_histogram_b_image.rows), cv::Point(start_x_b + m_i_histogmra_blue_max, 0), cv::Scalar(255, 0, 0), 2);

		for (int i = 0; i < (int)histogram_size; i++)
		{
			int value = 0;
			
			if (m_p_histogram != NULL)
			{
				value = m_p_histogram[i] * m_result_histogram_image.rows;
				cv::line(m_result_histogram_image, cv::Point(start_x_v + i, m_result_histogram_image.rows), cv::Point(start_x_v + i, m_result_histogram_image.rows - value), cv::Scalar(255, 255, 255), 1);
			}

			//r
			if (m_p_histogram_r != NULL)
			{
				value = m_p_histogram_r[i] * m_result_histogram_r_image.rows;
				cv::line(m_result_histogram_r_image, cv::Point(start_x_r + i, m_result_histogram_r_image.rows), cv::Point(start_x_r + i, m_result_histogram_r_image.rows - value), cv::Scalar(128, 128, 255), 1);
			}

			if (m_p_histogram_g != NULL)
			{
				value = m_p_histogram_g[i] * m_result_histogram_g_image.rows;
				cv::line(m_result_histogram_g_image, cv::Point(start_x_g + i, m_result_histogram_g_image.rows), cv::Point(start_x_g + i, m_result_histogram_g_image.rows - value), cv::Scalar(128, 255, 128), 1);
			}

			if (m_p_histogram_b != NULL)
			{
				value = m_p_histogram_b[i] * m_result_histogram_b_image.rows;
				cv::line(m_result_histogram_b_image, cv::Point(start_x_b + i, m_result_histogram_b_image.rows), cv::Point(start_x_b + i, m_result_histogram_b_image.rows - value), cv::Scalar(255, 128, 128), 1);
			}
		}

		//cv::imwrite("histogram.png", m_result_histogram_image);
		//cv::imwrite("histogram_r.png", m_result_histogram_r_image);
		//cv::imwrite("histogram_g.png", m_result_histogram_g_image);
		//cv::imwrite("histogram_b.png", m_result_histogram_b_image);

	}
}

#if 0
void CEyedeaCheckDefectTabDlg::OnBnClickedCheckOneOfSubs()
{
	// TODO: Add your control notification handler code here
	BOOL bCheckOptionOneOfSubs = IsDlgButtonChecked(IDC_CHECK_ONE_OF_SUBS);

	if (bCheckOptionOneOfSubs)
	{
		ERVS_SetVisionConfigOption(VISION_CONFIG_FIND_ONE_OF_SUBS, 1);
	}
	else
	{
		ERVS_SetVisionConfigOption(VISION_CONFIG_FIND_ONE_OF_SUBS, 0);
	}

	//check
	float only_one_of_subs = ERVS_GetVisionConfigOption(VISION_CONFIG_FIND_ONE_OF_SUBS);

	if (only_one_of_subs)
	{
		CheckDlgButton(IDC_CHECK_ONE_OF_SUBS, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_ONE_OF_SUBS, FALSE);
	}
}
#endif

BOOL CEyedeaCheckDefectTabDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;                // Do not process further
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CString strBaseID;
	GetDlgItem(IDC_EDIT_GEO_ID1)->GetWindowText(strBaseID);
	int base_id = _ttoi(strBaseID);

	CString strTargetID;
	GetDlgItem(IDC_EDIT_GEO_ID2)->GetWindowText(strTargetID);
	int target_id = _ttoi(strTargetID);

	CString strTolBase;
	GetDlgItem(IDC_EDIT_GEO_DISTANCE_TOL_BASE)->GetWindowText(strTolBase);
	float tol_base_value = _ttof(strTolBase);

	CString strTol;
	GetDlgItem(IDC_EDIT_GEO_DISTANCE_TOL)->GetWindowText(strTol);
	float tol_value = _ttof(strTol);

	float distance_value = 0;
	ERVS_Geometry_Distance(base_id, target_id, tol_base_value, tol_value, &distance_value);

	CString strDistance;
	strDistance.Format(_T("%f"), distance_value);
	GetDlgItem(IDC_EDIT_GEO_RESULT_DISTANCE)->SetWindowText(strDistance);

	//Get Result Image
	//Result image
	int len = 921600;
	//ERVS_GetImage(GET_IMAGE_RESULT, -1, (char**)&m_result_image.data, &len);
	ERVS_GetFindObjectResultImage(-1, -1, (char**)&m_result_image.data, &len);
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButtonGeoAngle()
{
	// TODO: Add your control notification handler code here

	CString strBaseID;
	GetDlgItem(IDC_EDIT_GEO_ID1)->GetWindowText(strBaseID);
	int base_id = _ttoi(strBaseID);

	CString strTargetID;
	GetDlgItem(IDC_EDIT_GEO_ID2)->GetWindowText(strTargetID);
	int target_id = _ttoi(strTargetID);

	CString strTolBase;
	GetDlgItem(IDC_EDIT_GEO_ANGLE_TOL_BASE)->GetWindowText(strTolBase);
	float tol_base_value = _ttof(strTolBase);

	CString strTol;
	GetDlgItem(IDC_EDIT_GEO_ANGLE_TOL)->GetWindowText(strTol);
	float tol_value = _ttof(strTol);

	float angle_value = 0;
	ERVS_Geometry_Angle(base_id, target_id, tol_base_value, tol_value, &angle_value);

	CString strAngle;
	strAngle.Format(_T("%f"), angle_value);
	GetDlgItem(IDC_EDIT_GEO_RESULT_ANGLE)->SetWindowText(strAngle);

	//Get Result Image
	//Result image
	int len = 921600;
	//ERVS_GetImage(GET_IMAGE_RESULT, -1, (char**)&m_result_image.data, &len);
	ERVS_GetFindObjectResultImage(-1, -1, (char**)&m_result_image.data, &len);
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButtonGeoResultClear()
{
	// TODO: Add your control notification handler code here
	ERVS_Geometry_Clear();

	//Get Result Image
	//Result image
	int len = 921600;
	//ERVS_GetImage(GET_IMAGE_RESULT, -1, (char**)&m_result_image.data, &len);
	ERVS_GetFindObjectResultImage(-1, -1, (char**)&m_result_image.data, &len);
}


void CEyedeaCheckDefectTabDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//IDC_STATIC_HISTOGRAM
	//IDC_STATIC_HISTOGRAM_R
	//IDC_STATIC_HISTOGRAM_G
	//IDC_STATIC_HISTOGRAM_B

	CRect rect_histogram_gray;
	GetDlgItem(IDC_STATIC_HISTOGRAM)->GetWindowRect(&rect_histogram_gray);
	ScreenToClient(&rect_histogram_gray);

	CRect rect_histogram_red;
	GetDlgItem(IDC_STATIC_HISTOGRAM_R)->GetWindowRect(&rect_histogram_red);
	ScreenToClient(&rect_histogram_red);

	CRect rect_histogram_green;
	GetDlgItem(IDC_STATIC_HISTOGRAM_G)->GetWindowRect(&rect_histogram_green);
	ScreenToClient(&rect_histogram_green);

	CRect rect_histogram_blue;
	GetDlgItem(IDC_STATIC_HISTOGRAM_B)->GetWindowRect(&rect_histogram_blue);
	ScreenToClient(&rect_histogram_blue);

	//gray
	if (point.x >= rect_histogram_gray.left && point.x <= rect_histogram_gray.right &&
		point.y >= rect_histogram_gray.top && point.y <= rect_histogram_gray.bottom)
	{
		m_i_histogram_lbdwn_object = 0;

		int value = point.x - rect_histogram_gray.left;
		float f_value = ((float)((float)value- m_f_start_x_v_on_ui) / m_f_histogram_w_scale_v)  ;
		
		m_i_histogmra_gray_min = f_value;

		if (m_i_histogmra_gray_min < 0) m_i_histogmra_gray_min = 0;
		else if (m_i_histogmra_gray_min > 255) m_i_histogmra_gray_min = 255;

		//printf("m_f_histogram_w_scale_v = %f\n", m_f_histogram_w_scale_v);
		//printf("m_f_start_x_v_on_ui = %f\n", m_f_start_x_v_on_ui);
		//printf("min value = %d, %f\n", value, f_value);

		ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_gray_min, m_i_histogmra_gray_max) ;
	}
	else if (point.x >= rect_histogram_red.left && point.x <= rect_histogram_red.right &&
			point.y >= rect_histogram_red.top && point.y <= rect_histogram_red.bottom)
	{
		m_i_histogram_lbdwn_object = 1;

		int value = point.x - rect_histogram_red.left;
		float f_value = ((float)((float)value - m_f_start_x_r_on_ui) / m_f_histogram_w_scale_r);

		m_i_histogmra_red_min = f_value;

		if (m_i_histogmra_red_min < 0) m_i_histogmra_red_min = 0;
		else if (m_i_histogmra_red_min > 255) m_i_histogmra_red_min = 255;

		ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_red_min, m_i_histogmra_red_max);

	}
	else if (point.x >= rect_histogram_green.left && point.x <= rect_histogram_green.right &&
			point.y >= rect_histogram_green.top && point.y <= rect_histogram_green.bottom)
	{
		m_i_histogram_lbdwn_object = 2;

		int value = point.x - rect_histogram_green.left;
		float f_value = ((float)((float)value - m_f_start_x_g_on_ui) / m_f_histogram_w_scale_g);

		m_i_histogmra_green_min = f_value;

		if (m_i_histogmra_green_min < 0) m_i_histogmra_green_min = 0;
		else if (m_i_histogmra_green_min > 255) m_i_histogmra_green_min = 255;

		ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_green_min, m_i_histogmra_green_max);

	}
	else if (point.x >= rect_histogram_blue.left && point.x <= rect_histogram_blue.right &&
			point.y >= rect_histogram_blue.top && point.y <= rect_histogram_blue.bottom)
	{
		m_i_histogram_lbdwn_object = 3;

		int value = point.x - rect_histogram_blue.left;
		float f_value = ((float)((float)value - m_f_start_x_b_on_ui) / m_f_histogram_w_scale_b);

		m_i_histogmra_blue_min = f_value;

		if (m_i_histogmra_blue_min < 0) m_i_histogmra_blue_min = 0;
		else if (m_i_histogmra_blue_min > 255) m_i_histogmra_blue_min = 255;

		ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_blue_min, m_i_histogmra_blue_max);
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CEyedeaCheckDefectTabDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_i_histogram_lbdwn_object == 0)	//gray
	{
		ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_gray_min, m_i_histogmra_gray_max);

		OnBnClickedButtonHistoGet();
	}
	else if (m_i_histogram_lbdwn_object == 1)	//red
	{
		ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_red_min, m_i_histogmra_red_max);

		OnBnClickedButtonHistoGet2();
	}
	else if (m_i_histogram_lbdwn_object == 2)	//green
	{
		ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_green_min, m_i_histogmra_green_max);

		OnBnClickedButtonHistoGet3();
	}
	else if (m_i_histogram_lbdwn_object == 3)	//blue
	{
		ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_blue_min, m_i_histogmra_blue_max);

		OnBnClickedButtonHistoGet4();
	}

	m_i_histogram_lbdwn_object = -1;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CEyedeaCheckDefectTabDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CRect rect_histogram_gray;
	GetDlgItem(IDC_STATIC_HISTOGRAM)->GetWindowRect(&rect_histogram_gray);
	ScreenToClient(&rect_histogram_gray);

	CRect rect_histogram_red;
	GetDlgItem(IDC_STATIC_HISTOGRAM_R)->GetWindowRect(&rect_histogram_red);
	ScreenToClient(&rect_histogram_red);

	CRect rect_histogram_green;
	GetDlgItem(IDC_STATIC_HISTOGRAM_G)->GetWindowRect(&rect_histogram_green);
	ScreenToClient(&rect_histogram_green);

	CRect rect_histogram_blue;
	GetDlgItem(IDC_STATIC_HISTOGRAM_B)->GetWindowRect(&rect_histogram_blue);
	ScreenToClient(&rect_histogram_blue);

	if (m_i_histogram_lbdwn_object == 0)	//gray
	{
		int value = point.x - rect_histogram_gray.left;
		float f_value = ((float)((float)value - m_f_start_x_v_on_ui) / m_f_histogram_w_scale_v);

		if (m_i_histogmra_gray_min < f_value)
		{
			m_i_histogmra_gray_max = f_value;

			if (m_i_histogmra_gray_max < 0) m_i_histogmra_gray_max = 0;
			else if (m_i_histogmra_gray_max > 255) m_i_histogmra_gray_max = 255;

			ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_gray_min, m_i_histogmra_gray_max);
		}
		else
		{
			m_i_histogmra_gray_max = -1;
		}
	}
	else if (m_i_histogram_lbdwn_object == 1)	//red
	{
		int value = point.x - rect_histogram_red.left;
		float f_value = ((float)((float)value - m_f_start_x_r_on_ui) / m_f_histogram_w_scale_r);

		if (m_i_histogmra_red_min < f_value)
		{
			m_i_histogmra_red_max = f_value;

			if (m_i_histogmra_red_max < 0) m_i_histogmra_red_max = 0;
			else if (m_i_histogmra_red_max > 255) m_i_histogmra_red_max = 255;

			ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_red_min, m_i_histogmra_red_max);
		}
		else
		{
			m_i_histogmra_red_max = -1;
		}
	}
	else if (m_i_histogram_lbdwn_object == 2)	//green
	{
		int value = point.x - rect_histogram_green.left;
		float f_value = ((float)((float)value - m_f_start_x_g_on_ui) / m_f_histogram_w_scale_g);

		if (m_i_histogmra_green_min < f_value)
		{
			m_i_histogmra_green_max = f_value;

			if (m_i_histogmra_green_max < 0) m_i_histogmra_green_max = 0;
			else if (m_i_histogmra_green_max > 255) m_i_histogmra_green_max = 255;

			ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_green_min, m_i_histogmra_green_max);
		}
		else
		{
			m_i_histogmra_green_max = -1;
		}
	}
	else if (m_i_histogram_lbdwn_object == 3)	//blue
	{
		int value = point.x - rect_histogram_blue.left;
		float f_value = ((float)((float)value - m_f_start_x_b_on_ui) / m_f_histogram_w_scale_b);

		if (m_i_histogmra_blue_min < f_value)
		{
			m_i_histogmra_blue_max = f_value;

			if (m_i_histogmra_blue_max < 0) m_i_histogmra_blue_max = 0;
			else if (m_i_histogmra_blue_max > 255) m_i_histogmra_blue_max = 255;

			ERVS_Histogram_Set_Range(m_select_id, m_i_histogram_lbdwn_object, m_i_histogmra_blue_min, m_i_histogmra_blue_max);
		}
		else
		{
			m_i_histogmra_blue_max = -1;
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoGet()
{
	// TODO: Add your control notification handler code here
	int min_value = 0;
	int max_value = 0;
	ERVS_Histogram_Get_Range(m_select_id, 0, &min_value, &max_value);

	CString str;
	str.Format(_T("%d"), min_value);
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MIN)->SetWindowText(str);

	str.Format(_T("%d"), max_value);
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MAX)->SetWindowText(str);
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoGet2()
{
	// TODO: Add your control notification handler code here
	int min_value = 0;
	int max_value = 0;
	ERVS_Histogram_Get_Range(m_select_id, 1, &min_value, &max_value);

	CString str;
	str.Format(_T("%d"), min_value);
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MIN2)->SetWindowText(str);

	str.Format(_T("%d"), max_value);
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MAX2)->SetWindowText(str);
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoGet3()
{
	// TODO: Add your control notification handler code here
	int min_value = 0;
	int max_value = 0;
	ERVS_Histogram_Get_Range(m_select_id, 2, &min_value, &max_value);

	CString str;
	str.Format(_T("%d"), min_value);
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MIN3)->SetWindowText(str);

	str.Format(_T("%d"), max_value);
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MAX3)->SetWindowText(str);
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoGet4()
{
	// TODO: Add your control notification handler code here
	int min_value = 0;
	int max_value = 0;
	ERVS_Histogram_Get_Range(m_select_id, 3, &min_value, &max_value);

	CString str;
	str.Format(_T("%d"), min_value);
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MIN4)->SetWindowText(str);

	str.Format(_T("%d"), max_value);
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MAX4)->SetWindowText(str);
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoSet()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MIN)->GetWindowText(str);
	int min_value = _ttoi(str);

	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MAX)->GetWindowText(str);
	int max_value = _ttoi(str);

	ERVS_Histogram_Set_Range(m_select_id, 0, min_value, max_value);

	OnBnClickedButtonHistoGet();
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoSet2()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MIN2)->GetWindowText(str);
	int min_value = _ttoi(str);

	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MAX2)->GetWindowText(str);
	int max_value = _ttoi(str);

	ERVS_Histogram_Set_Range(m_select_id, 1, min_value, max_value);

	OnBnClickedButtonHistoGet2();
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoSet3()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MIN3)->GetWindowText(str);
	int min_value = _ttoi(str);

	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MAX3)->GetWindowText(str);
	int max_value = _ttoi(str);

	ERVS_Histogram_Set_Range(m_select_id, 2, min_value, max_value);

	OnBnClickedButtonHistoGet3();
}


void CEyedeaCheckDefectTabDlg::OnBnClickedButtonHistoSet4()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MIN4)->GetWindowText(str);
	int min_value = _ttoi(str);

	GetDlgItem(IDC_EDIT_HISTOGRAM_RANGE_MAX4)->GetWindowText(str);
	int max_value = _ttoi(str);

	ERVS_Histogram_Set_Range(m_select_id, 3, min_value, max_value);

	OnBnClickedButtonHistoGet4();
}
