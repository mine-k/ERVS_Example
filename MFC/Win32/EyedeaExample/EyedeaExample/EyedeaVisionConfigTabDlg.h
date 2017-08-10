#pragma once

#include <boost/thread.hpp>														//eyedea - using thread on boost.
#include "../../../src/EyedeaSDK/CXX/EyedeaAPI.h"								//eyedea - draw lib.
#include "opencv2/opencv.hpp"													//eyedea - opencv lib.
#include "../../../lib/cvvImage/CvvImage.h"										//eyedea - draw lib.

#include "afxwin.h"

// CEyedeaVisionConfigTabDlg dialog

class CEyedeaVisionConfigTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEyedeaVisionConfigTabDlg)

public:
	CEyedeaVisionConfigTabDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEyedeaVisionConfigTabDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_VISION_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonVisionConfigGet();
	afx_msg void OnBnClickedButtonVisionConfigSet();
	virtual BOOL OnInitDialog();
	//CComboBox m_combo_object_type_for_env;
	//afx_msg void OnBnClickedButtonEnvOptionSearchAreaGet();
	//afx_msg void OnBnClickedButtonEnvOptionSearchAreaSet();
	//afx_msg void OnBnClickedButtonEnvOptionMasterAreaGet();
	//fx_msg void OnBnClickedButtonEnvOptionMasterAreaSet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectGet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectSet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectFeatureGet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectFeatureSet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectOutlineGet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectOutlineSet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectRectGet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectRectSet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectCenterGet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectCenterSet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectAngleGet();
	//afx_msg void OnBnClickedButtonEnvOptionObjectAngleSet();
	afx_msg void OnBnClickedButtonVisionConfigGet2();
	afx_msg void OnBnClickedButtonVisionConfigSet2();
	//afx_msg void OnBnClickedButtonConfigFileSave();
	//afx_msg void OnBnClickedButtonConfigFileLoad();
	afx_msg void OnBnClickedButtonCheckVisionConfigGlobal();
	bool m_b_draw_pause;

private:
	bool m_run_thread;
	boost::thread m_thread;
	void ThreadFunctionDraw();

	cv::Mat m_check_image;
	cv::Mat m_check_image2;
public:
	afx_msg void OnBnClickedButtonCheckVisionConfigLocal();
	afx_msg void OnBnClickedButtonApplyVisionConfigGlobal();
	afx_msg void OnBnClickedButtonApplyVisionConfigLocal();
	void RoadAllFromERVS();
	afx_msg void OnBnClickedButtonBgLearningStart();
};