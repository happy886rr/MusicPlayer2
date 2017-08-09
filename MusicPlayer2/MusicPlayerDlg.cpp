
// MusicPlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "MusicPlayerDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include "afxcmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	CStatic m_static_version;
	CToolTipCtrl m_Mytip;		//���ָ��ʱ�Ĺ�����ʾ

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_static_version);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��
	CString version_info;
	const wchar_t* version = VERSION;
#ifdef COMPILE_IN_WIN_XP
	version_info.Format(_T("MusicPlayer2��%s �� (for WinXP)"), version);
#else
	version_info.Format(_T("MusicPlayer2��%s ��"), version);
#endif
	m_static_version.SetWindowText(version_info);

	m_Mytip.Create(this);
	m_Mytip.AddTool(GetDlgItem(IDC_SYSLINK1), _T("mailto:zhongyang219@hotmail.com"));	//���ӹ�����ʾ
	m_Mytip.SetDelayTime(300);	//�����ӳ�

	//if (theApp.m_is_windows10)
	//	SetDlgItemText(IDC_DEBUG_INFO_STATIC, _T("Windows10"));
	//else
	//	SetDlgItemText(IDC_DEBUG_INFO_STATIC, _T("Not Windows10"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	ShellExecute(NULL, _T("open"), _T("mailto:zhongyang219@hotmail.com"), NULL, NULL, SW_SHOW);	//�򿪳�����
	*pResult = 0;
}

BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ�����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE)
		m_Mytip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}


// CMusicPlayerDlg �Ի���

//��̬��Ա��ʼ��
unsigned int CMusicPlayerDlg::m_WM_TASKBARCREATED{ ::RegisterWindowMessage(_T("TaskbarCreated")) };	//ע����������������Ϣ

CMusicPlayerDlg::CMusicPlayerDlg(wstring cmdLine, CWnd* pParent /*=NULL*/)
	: m_cmdLine{cmdLine}, CDialog(IDD_MUSICPLAYER2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMusicPlayerDlg::~CMusicPlayerDlg()
{
	//if (m_pMaskDlg != nullptr)
	//{
	//	m_pMaskDlg->OnCancel();
	//	delete m_pMaskDlg;
	//	m_pMaskDlg = nullptr;
	//}
	if (m_pLyricEdit != nullptr)
	{
		m_pLyricEdit->OnCancel();
		delete m_pLyricEdit;
		m_pLyricEdit = nullptr;
	}
	if (m_pLyricBatchDownDlg != nullptr)
	{
		m_pLyricBatchDownDlg->OnCancel();
		delete m_pLyricBatchDownDlg;
		m_pLyricBatchDownDlg = nullptr;
	}
}

void CMusicPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TRACK_EDIT, m_track_edit);
	DDX_Control(pDX, IDC_PLAYLIST_LIST, m_playlist_list);
	DDX_Control(pDX, IDC_PATH_STATIC, m_path_static);
	DDX_Control(pDX, IDC_PATH_EDIT, m_path_edit);
	DDX_Control(pDX, IDC_PLAY_PAUSE, m_play_pause_button);
	DDX_Control(pDX, IDC_STOP, m_stop_button);
	DDX_Control(pDX, IDC_PREVIOUS, m_previous_button);
	DDX_Control(pDX, IDC_NEXT, m_next_button);
	DDX_Control(pDX, IDC_PROGRESS_STATIC, m_progress_bar);
	DDX_Control(pDX, IDC_TIME_STATIC, m_time_static);
	DDX_Control(pDX, ID_SET_PATH, m_set_path_button);
}

BEGIN_MESSAGE_MAP(CMusicPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_PLAY_PAUSE, &CMusicPlayerDlg::OnPlayPause)
	ON_COMMAND(ID_STOP, &CMusicPlayerDlg::OnStop)
	ON_COMMAND(ID_PREVIOUS, &CMusicPlayerDlg::OnPrevious)
	ON_COMMAND(ID_NEXT, &CMusicPlayerDlg::OnNext)
	ON_COMMAND(ID_REW, &CMusicPlayerDlg::OnRew)
	ON_COMMAND(ID_FF, &CMusicPlayerDlg::OnFF)
	ON_COMMAND(ID_SET_PATH, &CMusicPlayerDlg::OnSetPath)
	ON_COMMAND(ID_FIND, &CMusicPlayerDlg::OnFind)
	ON_COMMAND(ID_EXPLORE_PATH, &CMusicPlayerDlg::OnExplorePath)
	ON_WM_DESTROY()
	ON_COMMAND(ID_APP_ABOUT, &CMusicPlayerDlg::OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, &CMusicPlayerDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_FOLDER, &CMusicPlayerDlg::OnFileOpenFolder)
	ON_WM_DROPFILES()
	ON_WM_INITMENU()
	ON_COMMAND(ID_PLAY_ORDER, &CMusicPlayerDlg::OnPlayOrder)
	ON_COMMAND(ID_PLAY_SHUFFLE, &CMusicPlayerDlg::OnPlayShuffle)
	ON_COMMAND(ID_LOOP_PLAYLIST, &CMusicPlayerDlg::OnLoopPlaylist)
	ON_COMMAND(ID_LOOP_TRACK, &CMusicPlayerDlg::OnLoopTrack)
	ON_WM_MOUSEWHEEL()
	//ON_BN_CLICKED(IDC_VOLUME_UP, &CMusicPlayerDlg::OnBnClickedVolumeUp)
	//ON_BN_CLICKED(IDC_VOLUME_DOWN, &CMusicPlayerDlg::OnBnClickedVolumeDown)
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(NM_DBLCLK, IDC_PLAYLIST_LIST, &CMusicPlayerDlg::OnNMDblclkPlaylistList)
	//ON_COMMAND(ID_REFRESH_PLAYLIST, &CMusicPlayerDlg::OnRefreshPlaylist)
	ON_COMMAND(ID_OPTION_SETTINGS, &CMusicPlayerDlg::OnOptionSettings)
	ON_COMMAND(ID_RELOAD_PLAYLIST, &CMusicPlayerDlg::OnReloadPlaylist)
	ON_NOTIFY(NM_RCLICK, IDC_PLAYLIST_LIST, &CMusicPlayerDlg::OnNMRClickPlaylistList)
	ON_COMMAND(ID_PLAY_ITEM, &CMusicPlayerDlg::OnPlayItem)
	ON_COMMAND(ID_ITEM_PROPERTY, &CMusicPlayerDlg::OnItemProperty)
	ON_COMMAND(ID_REMOVE_FROM_PLAYLIST, &CMusicPlayerDlg::OnRemoveFromPlaylist)
	//ON_COMMAND(ID_CLEAR_PLAYLIST, &CMusicPlayerDlg::OnClearPlaylist)
	ON_COMMAND(ID_EXPLORE_TRACK, &CMusicPlayerDlg::OnExploreTrack)
	ON_WM_HOTKEY()
	ON_STN_CLICKED(IDC_PROGRESS_STATIC, &CMusicPlayerDlg::OnStnClickedProgressStatic)
	ON_COMMAND(ID_RE_INI_BASS, &CMusicPlayerDlg::OnReIniBass)
	ON_COMMAND(ID_SORT_BY_FILE, &CMusicPlayerDlg::OnSortByFile)
	ON_COMMAND(ID_SORT_BY_TITLE, &CMusicPlayerDlg::OnSortByTitle)
	ON_COMMAND(ID_SORT_BY_ARTIST, &CMusicPlayerDlg::OnSortByArtist)
	ON_COMMAND(ID_SORT_BY_ALBUM, &CMusicPlayerDlg::OnSortByAlbum)
	ON_COMMAND(ID_SORT_BY_TRACK, &CMusicPlayerDlg::OnSortByTrack)
	ON_COMMAND(ID_DELETE_FROM_DISK, &CMusicPlayerDlg::OnDeleteFromDisk)
	ON_REGISTERED_MESSAGE(m_WM_TASKBARCREATED, &CMusicPlayerDlg::OnTaskbarcreated)
	ON_COMMAND(ID_DISP_FILE_NAME, &CMusicPlayerDlg::OnDispFileName)
	ON_COMMAND(ID_DISP_TITLE, &CMusicPlayerDlg::OnDispTitle)
	ON_COMMAND(ID_DISP_ARTIST_TITLE, &CMusicPlayerDlg::OnDispArtistTitle)
	ON_COMMAND(ID_DISP_TITLE_ARTIST, &CMusicPlayerDlg::OnDispTitleArtist)
	ON_COMMAND(ID_MINI_MODE, &CMusicPlayerDlg::OnMiniMode)
	ON_BN_CLICKED(IDC_STOP, &CMusicPlayerDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_PREVIOUS, &CMusicPlayerDlg::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_PLAY_PAUSE, &CMusicPlayerDlg::OnBnClickedPlayPause)
	ON_BN_CLICKED(IDC_NEXT, &CMusicPlayerDlg::OnBnClickedNext)
//	ON_WM_MOVE()
	ON_COMMAND(ID_RELOAD_LYRIC, &CMusicPlayerDlg::OnReloadLyric)
	ON_COMMAND(ID_SONG_INFO, &CMusicPlayerDlg::OnSongInfo)
	ON_COMMAND(ID_COPY_CURRENT_LYRIC, &CMusicPlayerDlg::OnCopyCurrentLyric)
	ON_COMMAND(ID_COPY_ALL_LYRIC, &CMusicPlayerDlg::OnCopyAllLyric)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_LYRIC_FORWARD, &CMusicPlayerDlg::OnLyricForward)
	ON_COMMAND(ID_LYRIC_DELAY, &CMusicPlayerDlg::OnLyricDelay)
	ON_COMMAND(ID_SAVE_MODIFIED_LYRIC, &CMusicPlayerDlg::OnSaveModifiedLyric)
	ON_COMMAND(ID_EDIT_LYRIC, &CMusicPlayerDlg::OnEditLyric)
	ON_COMMAND(ID_DOWNLOAD_LYRIC, &CMusicPlayerDlg::OnDownloadLyric)
	ON_COMMAND(ID_LYRIC_BATCH_DOWNLOAD, &CMusicPlayerDlg::OnLyricBatchDownload)
	ON_COMMAND(ID_DELETE_LYRIC, &CMusicPlayerDlg::OnDeleteLyric)
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_PLAYLIST_INI_COMPLATE, &CMusicPlayerDlg::OnPlaylistIniComplate)
END_MESSAGE_MAP()


// CMusicPlayerDlg ��Ϣ��������

void CMusicPlayerDlg::SaveConfig()
{
	CCommon::WritePrivateProfileIntW(L"config", L"window_width", m_window_width, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"window_hight", m_window_height, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"transparency", m_transparency, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"narrow_mode", m_narrow_mode, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"stop_when_error", m_stop_when_error, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"show_taskbar_progress", m_show_taskbar_progress, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"theme_color", theApp.m_theme_color, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"theme_color_follow_system", m_theme_color_follow_system, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"playlist_display_format", static_cast<int>(m_display_format), theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"show_lyric_in_cortana", m_show_lyric_in_cortana, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"save_lyric_in_offset", m_save_lyric_in_offset, theApp.m_config_path.c_str());
	WritePrivateProfileStringW(L"config",L"font", m_lyric_font_name.c_str(), theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"font_size", m_lyric_font_size, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"lyric_line_space", m_lyric_line_space, theApp.m_config_path.c_str());
}

void CMusicPlayerDlg::LoadConfig()
{
	m_window_width = GetPrivateProfileIntW(L"config", L"window_width", -1, theApp.m_config_path.c_str());
	m_window_height = GetPrivateProfileIntW(L"config", L"window_hight", -1, theApp.m_config_path.c_str());
	m_transparency = GetPrivateProfileInt(_T("config"), _T("transparency"), 100, theApp.m_config_path.c_str());
	m_narrow_mode = (GetPrivateProfileInt(_T("config"), _T("narrow_mode"), 0, theApp.m_config_path.c_str()) != 0);
	m_stop_when_error = (GetPrivateProfileInt(_T("config"), _T("stop_when_error"), 1, theApp.m_config_path.c_str()) != 0);
	m_show_taskbar_progress = (GetPrivateProfileInt(_T("config"), _T("show_taskbar_progress"), 1, theApp.m_config_path.c_str()) != 0);
	theApp.m_theme_color = GetPrivateProfileInt(_T("config"), _T("theme_color"), 16760187, theApp.m_config_path.c_str());
	m_theme_color_follow_system = (GetPrivateProfileInt(_T("config"), _T("theme_color_follow_system"), 1, theApp.m_config_path.c_str()) != 0);
	m_display_format = static_cast<DisplayFormat>(GetPrivateProfileInt(_T("config"), _T("playlist_display_format"), 2, theApp.m_config_path.c_str()));
	m_show_lyric_in_cortana = (GetPrivateProfileInt(_T("config"), _T("show_lyric_in_cortana"), 0, theApp.m_config_path.c_str()) != 0);
	m_save_lyric_in_offset = (GetPrivateProfileInt(_T("config"), _T("save_lyric_in_offset"), 1, theApp.m_config_path.c_str()) != 0);
	wchar_t buff[32];
	GetPrivateProfileStringW(L"config", L"font", L"΢���ź�", buff, sizeof(buff) / sizeof(wchar_t), theApp.m_config_path.c_str());
	m_lyric_font_name = buff;
	m_lyric_font_size = GetPrivateProfileIntW(L"config", L"font_size", 10, theApp.m_config_path.c_str());
	m_lyric_font.CreatePointFont(m_lyric_font_size * 10, m_lyric_font_name.c_str());
	m_lyric_line_space = GetPrivateProfileIntW(L"config", L"lyric_line_space", 2, theApp.m_config_path.c_str());
}

void CMusicPlayerDlg::SetTransparency()
{
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, m_transparency * 255 / 100, LWA_ALPHA);  //͸����ȡֵ��ΧΪ0~255
}

void CMusicPlayerDlg::DrawInfo(bool reset)
{
	//������Ϣ����ľ���
	CRect info_rect;
	if (!m_narrow_mode)
		info_rect = CRect{ CPoint{2*m_margin, m_control_bar_height + 2*m_margin}, CSize{m_client_width / 2 - 4 * m_margin, m_info_height2 - 3 * m_margin } };
	else
		info_rect = CRect{ CPoint{ 2*m_margin, m_control_bar_height + m_progress_bar_height + m_margin}, CSize{ m_client_width - 4 * m_margin, m_info_height - 3 * m_margin } };
	
	//���û����DC
	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(NULL);
	m_draw_rect = info_rect;		//��ͼ����
	if (!m_narrow_mode)
		m_draw_rect.bottom = m_client_height - m_margin;
	MemBitmap.CreateCompatibleBitmap(m_pDC, m_draw_rect.Width(), m_draw_rect.Height());
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	m_draw.SetDC(&MemDC);	//��m_draw�еĻ�ͼDC����Ϊ�����DC
	MemDC.FillSolidRect(0,0,m_draw_rect.Width(), m_draw_rect.Height(), GetSysColor(COLOR_BTNFACE));	//������DC�Ļ�ͼ�������Ի���ı�����ɫ

	//���������˻����ͼ����m_draw_rect�����Ͻǵ����˻�ͼ��ԭ��
	info_rect.MoveToXY(0, 0);

	//�����ʾ��Ϣ����ı���ɫ
	CDrawCommon::SetDrawArea(&MemDC, info_rect);
	MemDC.FillSolidRect(info_rect, RGB(255, 255, 255));

	CPoint text_start{ info_rect.left + m_spectral_size.cx + 2 * m_margin, info_rect.top + m_margin };		//�ı�����ʼ����
	int text_height{ DPI(18) };		//�ı��ĸ߶�

	//��ʾ������Ϣ
	m_draw.SetFont(GetFont());
	m_draw.SetBackColor(RGB(255, 255, 255));
	CRect tmp{ text_start, CSize{1,text_height} };
	wchar_t buff[16];
	if (theApp.m_player.m_loading)
	{
		tmp.right = info_rect.right - m_margin;
		static CDrawCommon::ScrollInfo scroll_info0;
		CString info;
		info.Format(_T("�ҵ�%d�׸��������ڶ�ȡ��Ƶ�ļ���Ϣ�������%d%%�����Ժ򡭡�"), theApp.m_player.GetSongNum(), theApp.m_player.m_thread_info.process_percent);
		m_draw.DrawScrollText(tmp, info, m_text_color, static_cast<int>(DPI(1.5)), false, scroll_info0, reset);
	}
	else
	{
		//��ʾ���ڲ��ŵ��ļ���
		tmp.MoveToX(text_start.x + DPI(82));
		tmp.right = info_rect.right - m_margin;
		static CDrawCommon::ScrollInfo scroll_info1;
		m_draw.DrawScrollText(tmp, theApp.m_player.GetFileName().c_str(), m_text_color, static_cast<int>(DPI(1.5)), false, scroll_info1, reset);
		//��ʾ���ڲ��ŵ���Ŀ���
		tmp.MoveToX(text_start.x + DPI(52));
		tmp.right = tmp.left + DPI(30);
		swprintf_s(buff, sizeof(buff) / 2, L"%.3d", theApp.m_player.GetIndex() + 1);
		m_draw.DrawWindowText(tmp, buff, m_lyric_back_color, false);
		//��ʾ����״̬
		tmp.MoveToX(text_start.x);
		tmp.right = tmp.left + DPI(52);
		m_draw.DrawWindowText(tmp, theApp.m_player.GetPlayingState().c_str(), theApp.m_theme_color, false);
	}

	//��ʾ����
	tmp.MoveToXY(text_start.x, text_start.y + text_height);
	tmp.right = tmp.left + DPI(52);
	m_draw.DrawWindowText(tmp, _T("���⣺"), theApp.m_theme_color, false);
	tmp.MoveToX(tmp.left + DPI(52));
	tmp.right = info_rect.right - m_margin;
	static CDrawCommon::ScrollInfo scroll_info2;
	m_draw.DrawScrollText2(tmp, theApp.m_player.GetCurrentSongInfo().title.c_str(), m_text_color, DPI(1), false, scroll_info2, reset);
	//��ʾ������
	tmp.MoveToXY(text_start.x, text_start.y + 2 * text_height);
	tmp.right = tmp.left + DPI(52);
	m_draw.DrawWindowText(tmp, _T("�����ң�"), theApp.m_theme_color, false);
	tmp.MoveToX(tmp.left + DPI(52));
	tmp.right = info_rect.right - m_margin;
	static CDrawCommon::ScrollInfo scroll_info3;
	m_draw.DrawScrollText2(tmp, theApp.m_player.GetCurrentSongInfo().artist.c_str(), m_text_color, DPI(1), false, scroll_info3, reset);
	//��ʾ��Ƭ��
	tmp.MoveToXY(text_start.x, text_start.y + 3 * text_height);
	tmp.right = tmp.left + DPI(52);
	m_draw.DrawWindowText(tmp, _T("��Ƭ����"), theApp.m_theme_color, false);
	tmp.MoveToX(tmp.left + DPI(52));
	tmp.right = info_rect.right - m_margin;
	static CDrawCommon::ScrollInfo scroll_info4;
	m_draw.DrawScrollText2(tmp, theApp.m_player.GetCurrentSongInfo().album.c_str(), m_text_color, DPI(1), false, scroll_info4, reset);
	//��ʾ������
	tmp.MoveToXY(text_start.x, text_start.y + 4 * text_height);
	tmp.right = tmp.left + DPI(52);
	m_draw.DrawWindowText(tmp, _T("�����ʣ�"), theApp.m_theme_color, false);
	tmp.MoveToX(tmp.left + DPI(52));
	tmp.right = info_rect.right - m_margin;
	swprintf_s(buff, L"%d kbps", theApp.m_player.GetCurrentSongInfo().bitrate);
	m_draw.DrawWindowText(tmp, buff, m_text_color, false);

	//��ʾƵ�׷���
	CRect spectral_rect{ CPoint{info_rect.left + m_margin, info_rect.top + m_margin}, m_spectral_size };
	CDrawCommon::SetDrawArea(&MemDC, spectral_rect);
	MemDC.FillSolidRect(spectral_rect, m_text_back_color);
	const int ROWS = 16;		//Ƶ�����ε�����
	CRect rects[ROWS];
	int width = (spectral_rect.Width() - ROWS + 1) / ROWS;
	rects[0] = spectral_rect;
	rects[0].DeflateRect(m_margin / 2, m_margin / 2);
	rects[0].right = rects[0].left + width;
	for (int i{ 1 }; i < ROWS; i++)
	{
		rects[i] = rects[0];
		rects[i].left += (i * (width + 1));
		rects[i].right += (i * (width + 1));
	}
	for (int i{}; i < ROWS; i++)
	{
		CRect rect_tmp{ rects[i] };
		int spetral_height = static_cast<int>(theApp.m_player.GetSpectralData()[i] * rects[0].Height() / 30);
		if (spetral_height <= 0 || theApp.m_player.IsError()) spetral_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų�����Ҳ����ʾƵ��
		rect_tmp.top = rect_tmp.bottom - spetral_height;
		if (rect_tmp.top < rects[0].top) rect_tmp.top = rects[0].top;
		MemDC.FillSolidRect(rect_tmp, theApp.m_theme_color);
	}

	//��ʾ����������Ϣ
	//���Ʊ���
	CPoint point{ spectral_rect.left, spectral_rect.bottom };
	point.y += 2 * m_margin;
	CRect other_info_rect{ point, CSize(info_rect.Width() - 2 * m_margin,DPI(24)) };
	CDrawCommon::SetDrawArea(&MemDC, other_info_rect);
	MemDC.FillSolidRect(other_info_rect, m_text_back_color);
	//��ʾ������Ϣ
	m_draw.SetBackColor(m_text_back_color);
	//��ʾѭ��ģʽ
	tmp = other_info_rect;
	tmp.left += m_margin;
	tmp.right = tmp.left + DPI(112);
	m_repetemode_rect = tmp;
	m_repetemode_rect.DeflateRect(0, DPI(4));
	CString repeat_mode_str{_T("ѭ��ģʽ��")};
	switch (theApp.m_player.GetRepeatMode())
	{
	case RepeatMode::RM_PLAY_ORDER: repeat_mode_str += _T("˳�򲥷�"); break;
	case RepeatMode::RM_LOOP_PLAYLIST: repeat_mode_str += _T("�б�ѭ��"); break;
	case RepeatMode::RM_LOOP_TRACK: repeat_mode_str += _T("����ѭ��"); break;
	case RepeatMode::RM_PLAY_SHUFFLE: repeat_mode_str += _T("�������"); break;
	}
	if (m_repetemode_hover)		//���ָ��ѭ��ģʽ��ʱ��������һ����ɫ��ʾ
		m_draw.DrawWindowText(m_repetemode_rect, repeat_mode_str, m_lyric_back_color, false);
	else
		m_draw.DrawWindowText(m_repetemode_rect, repeat_mode_str, m_text_color, false);
	m_repetemode_rect.MoveToXY(CPoint{ m_repetemode_rect.left + m_draw_rect.left, m_repetemode_rect.top + m_draw_rect.top });	//����������任Ϊ�Կͻ������Ͻ�Ϊԭ��
	//��ʾ����
	tmp.MoveToX(info_rect.right - DPI(124));
	tmp.right = info_rect.right - DPI(49);
	m_volume_rect = tmp;
	m_volume_rect.DeflateRect(0, DPI(4));
	m_volume_rect.right -= DPI(12);
	swprintf_s(buff, L"������%d%%", theApp.m_player.GetVolume());
	if (m_volume_hover)		//���ָ����������ʱ��������һ����ɫ��ʾ
		m_draw.DrawWindowText(tmp, buff, m_lyric_back_color, false);
	else
		m_draw.DrawWindowText(tmp, buff, m_text_color, false);
	//��������������ť��λ��
	m_volume_down_rect = m_volume_rect;
	m_volume_down_rect.bottom += DPI(4);
	m_volume_down_rect.MoveToY(m_volume_rect.bottom);
	m_volume_down_rect.right = m_volume_rect.left + m_volume_rect.Width() / 2;
	m_volume_up_rect = m_volume_down_rect;
	m_volume_up_rect.MoveToX(m_volume_down_rect.right);
	m_volume_rect.MoveToXY(CPoint{ m_volume_rect.left + m_draw_rect.left, m_volume_rect.top + m_draw_rect.top });	//����������任Ϊ�Կͻ������Ͻ�Ϊԭ��
	//��ʾ<<<<��ÿ4��һ�����ڣ�
	int progress;
	Time time{ theApp.m_player.GetCurrentPosition() };
	progress = (time.sec % 4 * 1000 + time.msec) / 4;
	tmp.MoveToX(tmp.right);
	tmp.right = other_info_rect.right;
	m_draw.DrawWindowText(tmp, _T("<<<<"), m_text_color, m_lyric_back_color, progress, false);

	//��ʾ���
	m_draw.SetFont(&m_lyric_font);
	CRect lyric_rect;
	if (m_narrow_mode)
	{
		lyric_rect = other_info_rect;
		lyric_rect.MoveToY(other_info_rect.bottom + m_margin);
		DrawLyricsSingleLine(lyric_rect);
	}
	else
	{
		//if (theApp.m_player.IsPlaying() || reset)
		//{
		lyric_rect = info_rect;
		lyric_rect.MoveToY(info_rect.bottom + 2*m_margin);
		lyric_rect.bottom = m_draw_rect.Height() - m_margin;
		DrawLyricsMulityLine(lyric_rect, &MemDC);
		//}
	}

	//������������ť����Ϊ�������ϲ㣬���Ա����ڸ�ʻ�����ɺ����
	if (m_show_volume_adj)
	{
		m_draw.SetBackColor(m_lyric_back_color);
		m_draw.DrawWindowText(m_volume_down_rect, L"-", RGB(255, 255, 255), true);
		m_draw.DrawWindowText(m_volume_up_rect, L"+", RGB(255, 255, 255), true);
	}
	m_volume_down_rect.MoveToXY(CPoint{ m_volume_down_rect.left + m_draw_rect.left, m_volume_down_rect.top + m_draw_rect.top });	//����������任Ϊ�Կͻ������Ͻ�Ϊԭ��
	m_volume_up_rect.MoveToXY(CPoint{ m_volume_up_rect.left + m_draw_rect.left, m_volume_up_rect.top + m_draw_rect.top });	//����������任Ϊ�Կͻ������Ͻ�Ϊԭ��

	//��������DC�е�ͼ�񿽱�����Ļ����ʾ
	m_pDC->BitBlt(m_draw_rect.left, m_draw_rect.top, m_draw_rect.Width(), m_draw_rect.Height(), &MemDC, 0, 0, SRCCOPY);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

void CMusicPlayerDlg::DrawLyricsSingleLine(CRect lyric_rect)
{
	if (theApp.m_player.m_Lyrics.IsEmpty())
	{
		m_draw.DrawWindowText(lyric_rect, _T("��ǰ����û�и��"), m_lyric_back_color, true);
	}
	else
	{
		wstring current_lyric{ theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 0) };	//��ȡ�����
		if (current_lyric.empty())		//�����ǰ���Ϊ�հף�����ʾΪʡ�Ժ�
			current_lyric = DEFAULT_LYRIC_TEXT;
		if (theApp.m_player.m_lyric_karaoke_disp)		//����Կ���OK��ʽ��ʾʱ
		{
			int progress{ theApp.m_player.m_Lyrics.GetLyricProgress(Time(theApp.m_player.GetCurrentPosition())) };		//��ȡ��ǰ��ʽ��ȣ���ΧΪ0~1000��
			m_draw.DrawWindowText(lyric_rect, current_lyric.c_str(), m_text_color, m_lyric_back_color, progress, true);
		}
		else				//��ʲ��Կ���OK��ʽ��ʾʱ
		{
			m_draw.DrawWindowText(lyric_rect, current_lyric.c_str(), m_text_color, true);
		}
	}
}

void CMusicPlayerDlg::DrawLyricsMulityLine(CRect lyric_rect, CDC* pDC)
{
	//����ɫ����
	CDrawCommon::SetDrawArea(pDC, lyric_rect);
	pDC->FillSolidRect(lyric_rect, RGB(255, 255, 255));
	//��ʾ������㡱
	CRect tmp;
	tmp = lyric_rect;
	tmp.left += 2*m_margin;
	tmp.bottom = tmp.top + DPI(28);
	m_draw.SetBackColor(RGB(255, 255, 255));
	m_draw.SetFont(GetFont());
	m_draw.DrawWindowText(tmp, _T("����㣺"), m_text_color, false);
	m_draw.SetFont(&m_lyric_font);
	//��������򱳾�ɫ
	CRect lyric_area = lyric_rect;
	lyric_area.DeflateRect(2 * m_margin, 2 * m_margin);
	lyric_area.top += DPI(20);
	CDrawCommon::SetDrawArea(pDC, lyric_area);
	pDC->FillSolidRect(lyric_area, m_text_back_color);
	//���ø����������
	lyric_area.DeflateRect(2 * m_margin, 2 * m_margin);
	CDrawCommon::SetDrawArea(pDC, lyric_area);
	//���Ƹ���ı�
	m_draw.SetBackColor(m_text_back_color);
	//�����ı��߶�
	m_pDC->SelectObject(&m_lyric_font);
	int lyric_height = m_pDC->GetTextExtent(L"��").cy;	//���ݵ�ǰ���������ü����ı��ĸ߶�
	lyric_height += m_lyric_line_space;			//�ı��߶ȼ����м��
	if (theApp.m_player.m_Lyrics.IsEmpty())
	{
		m_draw.DrawWindowText(lyric_area, _T("��ǰ����û�и��"), m_lyric_back_color, true);
	}
	else
	{
		CRect arect{ lyric_area };
		arect.bottom = arect.top + lyric_height;
		vector<CRect> rects(theApp.m_player.m_Lyrics.GetLyricCount() + 1, arect);		//Ϊÿһ���ʴ���һ�����Σ�������������
		int center_pos = (lyric_area.top + lyric_area.bottom) / 2;		//������������y����
		Time time{ theApp.m_player.GetCurrentPosition() };		//��ǰ����ʱ��
		int lyric_index = theApp.m_player.m_Lyrics.GetLyricIndex(time);		//��ǰ��ʵ����
		int progress = theApp.m_player.m_Lyrics.GetLyricProgress(time);		//��ǰ��ʽ��ȣ���ΧΪ0~1000��
		int y_progress = progress*lyric_height / 1000;			//��ǰ�����y���ϵĽ���
		int start_pos = center_pos - y_progress - (lyric_index + 1)*lyric_height;		//��1���ʵ���ʼy����
		for (int i{}; i<rects.size(); i++)
		{
			//����ÿһ���ʵ�λ��
			if (i == 0)
				rects[i].MoveToY(start_pos);
			else
				rects[i].MoveToY(rects[i - 1].bottom);
			//���Ƹ���ı�
			if (!(rects[i] & lyric_area).IsRectEmpty())		//ֻ�е�һ���ʵľ�������͸������ľ����н���ʱ���Ż��Ƹ��
			{
				if (i == lyric_index + 1)		//�������ڲ��ŵĸ��
				{
					if (theApp.m_player.m_lyric_karaoke_disp)
						m_draw.DrawWindowText(rects[i], theApp.m_player.m_Lyrics.GetLyric(i).c_str(), m_text_color, m_lyric_back_color, progress, true, true);
					else
						m_draw.DrawWindowText(rects[i], theApp.m_player.m_Lyrics.GetLyric(i).c_str(), m_text_color, true, true);
				}
				else		//���Ʒ����ڲ��ŵĸ��
				{
					m_draw.DrawWindowText(rects[i], theApp.m_player.m_Lyrics.GetLyric(i).c_str(), m_lyric_back_color, true, true);
				}
			}
		}
	}
}

void CMusicPlayerDlg::SetPlaylistSize(int cx, int cy)
{
	//���ò����б���С
	int width0, width1, width2;
	if (!m_narrow_mode)
	{
		m_playlist_list.MoveWindow(cx / 2 + m_margin, m_control_bar_height + m_path_edit_height + m_margin, cx / 2 - 2 * m_margin, cy - m_control_bar_height - m_path_edit_height - 2 * m_margin);
		width0 = DPI(40);
		width2 = DPI(50);
		width1 = cx / 2 - width0 - width2 - DPI(21) - 2 * m_margin;
	}
	else
	{
		m_playlist_list.MoveWindow(m_margin, m_control_bar_height + m_info_height + m_progress_bar_height + m_path_edit_height + m_margin, cx - 2 * m_margin, cy - m_control_bar_height - m_info_height - m_progress_bar_height - m_path_edit_height - 2 * m_margin);
		width0 = DPI(40);
		width2 = DPI(50);
		width1 = cx - width0 - width2 - DPI(21) - 2 * m_margin;
	}
	m_playlist_list.SetColumnWidth(0, width0);
	m_playlist_list.SetColumnWidth(1, width1);
	m_playlist_list.SetColumnWidth(2, width2);
	//���á���ǰ·����static�ؼ���С
	CRect rect_static;
	m_path_static.GetWindowRect(rect_static);
	rect_static.bottom = rect_static.top + m_path_edit_height;
	if (!m_narrow_mode)
		rect_static.MoveToXY(cx / 2 + m_margin, m_control_bar_height);
	else
		rect_static.MoveToXY(m_margin, m_control_bar_height + m_info_height + m_progress_bar_height);
	m_path_static.MoveWindow(rect_static);
	//���á���ǰ·����edit�ؼ���С
	CRect rect_edit;
	m_path_edit.GetWindowRect(rect_edit);
	if (!m_narrow_mode)
	{
		rect_edit.right = rect_edit.left + (cx / 2 - 2 * m_margin - rect_static.Width());
		rect_edit.MoveToXY(cx / 2 + m_margin + rect_static.Width(), m_control_bar_height + m_margin);
	}
	else
	{
		rect_edit.right = rect_edit.left + (cx - 2 * m_margin - rect_static.Width());
		rect_edit.MoveToXY(m_margin + rect_static.Width(), m_control_bar_height + m_info_height + m_progress_bar_height + m_margin);
	}
	m_path_edit.MoveWindow(rect_edit);
}

void CMusicPlayerDlg::SetPorgressBarSize(int cx, int cy)
{
	CRect rect;
	m_progress_bar.GetWindowRect(rect);
	int left_pos;		//���������λ��
	int progress_width;	//����������
	int time_width;	//ʱ��Ŀ���
	if (theApp.m_player.GetSongLength() >= 6000000)	//����������ȴ���6000000���룬������100���ӣ�����ʾʱ��ؼ��Ŀ�������50%
		time_width = m_time_width * 3 / 2;
	else if (theApp.m_player.GetSongLength() >= 600000)	//����������ȴ���600000���룬������10���ӣ�����ʾʱ��ؼ��Ŀ�������25%
		time_width = m_time_width * 5 / 4;
	else
		time_width = m_time_width;
	if (!m_narrow_mode)
	{
		//���ý�����λ��
		left_pos = m_progress_bar_left_pos;
		progress_width = cx - left_pos - 2 * m_margin - time_width;
		rect.right = rect.left + progress_width;
		rect.MoveToXY(left_pos, DPI(6));
		m_progress_bar.MoveWindow(rect);
		//����ʱ��λ��
		m_time_static.SetWindowPos(NULL, cx - time_width - m_margin, DPI(6), time_width, m_time_height, SWP_NOZORDER);
	}
	else
	{
		//���ý�����λ��
		left_pos = m_margin;
		progress_width = cx - 3 * m_margin - time_width;
		rect.right = rect.left + progress_width;
		rect.MoveToXY(left_pos, DPI(6) + m_control_bar_height);
		m_progress_bar.MoveWindow(rect);
		//����ʱ��λ��
		m_time_static.SetWindowPos(NULL, cx - time_width - m_margin, DPI(6) + m_control_bar_height, time_width, m_time_height, SWP_NOZORDER);
	}
}

void CMusicPlayerDlg::SetPorgressBarSize()
{
	CRect rect;
	GetClientRect(rect);
	SetPorgressBarSize(rect.Width(), rect.Height());
}

void CMusicPlayerDlg::ShowPlayList()
{
	m_playlist_list.ShowPlaylist(m_display_format);

	//���ò����б���ͻ����ʾ����Ŀ
	SetPlayListColor();
	//��ʾ��ǰ·��
	m_path_edit.SetWindowTextW(theApp.m_player.GetCurrentPath().c_str());

	m_progress_bar.SetSongLength(theApp.m_player.GetSongLength());

	if (m_miniModeDlg.m_hWnd != NULL)
		m_miniModeDlg.ShowPlaylist();
}

void CMusicPlayerDlg::SetPlayListColor()
{
	//m_playlist_list.ResetAllItemColor();
	//m_playlist_list.SetItemColor(theApp.m_player.GetIndex(), m_text_color, m_text_back_color);
	m_playlist_list.SetHightItem(theApp.m_player.GetIndex());
	m_playlist_list.SetColor(m_text_color, m_text_back_color, theApp.m_theme_color, m_lyric_back_color);
	m_playlist_list.EnsureVisible(theApp.m_player.GetIndex(), FALSE);
}

void CMusicPlayerDlg::SwitchTrack()
{
	//���л����ڲ��ŵĸ���ʱ���ò����б���ͻ����ʾ����Ŀ
	SetPlayListColor();
	if (m_miniModeDlg.m_hWnd != NULL)
	{
		m_miniModeDlg.SetPlayListColor();
		m_miniModeDlg.RePaint();
	}
	//�л�����ʱ�����ǰ������ʱ��û����ʾ������ʾ����
	CString song_length_str;
	int index{ theApp.m_player.GetIndex() };
	song_length_str = m_playlist_list.GetItemText(index, 2);
	if (song_length_str == _T("-:--") && !theApp.m_player.GetAllSongLength(index).isZero())
	{
		m_playlist_list.SetItemText(index, 2, theApp.m_player.GetAllSongLength(index).time2str().c_str());
	}
	ShowTime();
	m_progress_bar.SetSongLength(theApp.m_player.GetSongLength());
	SetPorgressBarSize();		//�����������ڴ����еĴ�С��λ��
	DrawInfo(true);
}

void CMusicPlayerDlg::ShowTime()
{
	m_time_static.SetWindowText(theApp.m_player.GetTimeString().c_str());
	//m_time_static.SetTextColor(m_text_color);
	//m_time_static.DrawWindowText(theApp.m_player.GetTimeString().c_str());
}

void CMusicPlayerDlg::UpdateProgress()
{
	int position, length;
	position = theApp.m_player.GetCurrentPosition();
	length = theApp.m_player.GetSongLength();
	if (length == 0) length = 1;	//��ֹ����Ϊ0
	__int64 pos;	//�������Ŀ���
	pos = static_cast<__int64>(position) * 1000 / length;	//��ת����__int64���ͣ���ֹ����1000֮�����
	m_progress_bar.SetPos(static_cast<int>(pos));
}

void CMusicPlayerDlg::UpdateTaskBarProgress()
{
	//���ݲ���״̬����������״̬�ͽ���
	if (m_show_taskbar_progress)
	{
		int position, length;
		position = theApp.m_player.GetCurrentPosition();
		length = theApp.m_player.GetSongLength();
		if (position == 0) position = 1;
		if (length == 0) length = 1;
		if (theApp.m_player.IsError())
			m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_ERROR);
		else if (theApp.m_player.IsPlaying())
			m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_INDETERMINATE);
		else
			m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_PAUSED);
		m_pTaskbar->SetProgressValue(this->GetSafeHwnd(), position, length);
	}
	else
	{
		m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_NOPROGRESS);
	}
}

void CMusicPlayerDlg::UpdatePlayPauseButton()
{
	if (theApp.m_player.IsPlaying() && !theApp.m_player.IsError())
	{
		//��������������ͼ�ϡ�����/��ͣ����ͼ��
		m_thumbButton[1].hIcon = m_hPauseIcon;
		wcscpy_s(m_thumbButton[1].szTip, _T("��ͣ"));
		//�����������ϡ�����/��ͣ����ͼ��
		m_play_pause_button.SetImage(m_hPauseIcon_s, FALSE);
		m_play_pause_button.SetWindowText(_T("��ͣ(&P)"));
	}
	else
	{
		//��������������ͼ�ϡ�����/��ͣ����ͼ��
		m_thumbButton[1].hIcon = m_hPlayIcon;
		wcscpy_s(m_thumbButton[1].szTip, _T("����"));
		//�����������ϡ�����/��ͣ����ͼ��
		m_play_pause_button.SetImage(m_hPlayIcon_s, FALSE);
		m_play_pause_button.SetWindowText(_T("����(&P)"));
	}
	m_pTaskbar->ThumbBarUpdateButtons(m_hWnd, 3, m_thumbButton);
	if (m_miniModeDlg.m_hWnd != NULL)
		m_miniModeDlg.UpdatePlayPauseButton();
}

void CMusicPlayerDlg::SetThumbnailClipArea()
{
	CRect info_rect;
	if (!m_narrow_mode)
		info_rect = CRect{ CPoint{ 2 * m_margin, m_control_bar_height + 2 * m_margin + DPI(20) }, CSize{ m_client_width / 2 - 4 * m_margin, m_info_height2 - 3 * m_margin } };
	else
		info_rect = CRect{ CPoint{ 2 * m_margin, m_control_bar_height + m_progress_bar_height + m_margin + DPI(20) }, CSize{ m_client_width - 4 * m_margin, m_info_height - 3 * m_margin } };
	if (m_pTaskbar != nullptr)
		m_pTaskbar->SetThumbnailClip(m_hWnd, info_rect);
}

void CMusicPlayerDlg::DrawCortanaText(LPCTSTR str, bool reset)
{
	if (theApp.m_is_windows10 && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		//::SetWindowText(m_cortana_hwnd, str);
		//::SendMessage(m_cortana_hwnd, WM_SETTEXT, 0, (LPARAM)str);

		//���û����DC
		CDC MemDC;
		CBitmap MemBitmap;
		MemDC.CreateCompatibleDC(NULL);
		MemBitmap.CreateCompatibleBitmap(m_cortana_pDC, m_cortana_rect.Width(), m_cortana_rect.Height());
		CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
		//ʹ��m_cortana_draw��ͼ
		m_cortana_draw.SetDC(&MemDC);
		static CDrawCommon::ScrollInfo cortana_scroll_info;
		m_cortana_draw.DrawScrollText(m_cortana_rect, str, RGB(255, 255, 255),DPI(2), false, cortana_scroll_info, reset);
		//��������DC�е�ͼ�񿽱�����Ļ����ʾ
		m_cortana_pDC->BitBlt(0, 0, m_cortana_rect.Width(), m_cortana_rect.Height(), &MemDC, 0, 0, SRCCOPY);
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
	}
}

void CMusicPlayerDlg::DrawCortanaText(LPCTSTR str, int progress)
{
	if (theApp.m_is_windows10 && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		//���û����DC
		CDC MemDC;
		CBitmap MemBitmap;
		MemDC.CreateCompatibleDC(NULL);
		MemBitmap.CreateCompatibleBitmap(m_cortana_pDC, m_cortana_rect.Width(), m_cortana_rect.Height());
		CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
		//ʹ��m_cortana_draw��ͼ
		m_cortana_draw.SetDC(&MemDC);
		m_cortana_draw.DrawWindowText(m_cortana_rect, str, RGB(255, 255, 255), m_lyric_back_color, progress, false, true);
		//��������DC�е�ͼ�񿽱�����Ļ����ʾ
		m_cortana_pDC->BitBlt(0, 0, m_cortana_rect.Width(), m_cortana_rect.Height(), &MemDC, 0, 0, SRCCOPY);
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
	}
}

void CMusicPlayerDlg::ResetCortanaText()
{
	if (theApp.m_is_windows10 && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		DrawCortanaText(m_cortana_default_text.c_str(), true);
		m_cortana_wnd->Invalidate();
	}
}

void CMusicPlayerDlg::GetCortanaHandle()
{
	if (theApp.m_is_windows10)
	{
		HWND hTaskBar = ::FindWindow(_T("Shell_TrayWnd"), NULL);	//�������ľ��
		HWND hCortanaBar = ::FindWindowEx(hTaskBar, NULL, _T("TrayDummySearchControl"), NULL);	//Cortana���ľ�������а���3���Ӵ��ڣ�
		HWND hCortanaButton = ::FindWindowEx(hCortanaBar, NULL, _T("Button"), NULL);	//Cortana������������Ϊ��Button���Ĵ��ڵľ��
		m_cortana_hwnd = ::FindWindowEx(hCortanaBar, NULL, _T("Static"), NULL);		//Cortana������������Ϊ��Static���Ĵ��ڵľ��
		if (m_cortana_hwnd == NULL) return;
		wchar_t buff[32];
		::GetWindowText(hCortanaButton, buff, 31);		//��ȡCortana��������ԭ�����ַ����������ڳ����˳�ʱ�ָ�
		m_cortana_default_text = buff;
		m_cortana_wnd = FromHandle(m_cortana_hwnd);		//��ȡCortana�������CWnd���ָ��
		if (m_cortana_wnd == nullptr) return;
		::GetClientRect(m_cortana_hwnd, m_cortana_rect);	//��ȡCortana������ľ�������
		m_cortana_pDC = m_cortana_wnd->GetDC();
		m_cortana_draw.Create(m_cortana_pDC, m_cortana_wnd);
		m_cortana_draw.SetBackColor(RGB(47, 47, 47));	//���û�ͼ�ı�����ɫ
		m_cortana_draw.SetFont(&m_cortana_font);		//��������
	}
}

BOOL CMusicPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_MINIMODE,_T("����ģʽ(&I)\tCtrl+M"));

		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

	//��������
	LoadConfig();

	//ֻ��win8���ϵ�ϵͳ���ܸ���ϵͳ����ɫ
#ifdef COMPILE_IN_WIN_XP
	m_theme_color_follow_system = false;
#else
	if (!IsWindows8OrGreater())
		m_theme_color_follow_system = false;
#endif

	//ֻ��Win10���ϵ�ϵͳ������Cortana����������ʾ���
	if (!theApp.m_is_windows10)
		m_show_lyric_in_cortana = false;

	//���ô�����ʽ
	//DWORD dwStyle{};
	////����windows10ʱΪ��������WS_CLIPCHILDREN���ԣ��Խ�����ڴ�С�仯ʱ����̨������ʾ������������
	//if (!theApp.m_is_windows10)
	//{
	//	dwStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	//	dwStyle |= WS_CLIPCHILDREN;
	//	SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, dwStyle);
	//}

	//���ô��ڲ�͸����
	SetTransparency();

	//��ʼ�����ڴ�С
	CRect rect;
	rect.right = m_window_width;
	rect.bottom = m_window_height;
	if (m_window_height != -1 && m_window_width != -1)
		MoveWindow(rect);

	//��ȡ��ǰϵͳDPI����
	CWindowDC dc(this);
	HDC hDC = dc.GetSafeHdc();
	theApp.m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);

	//���ݵ�ǰϵͳ��DPI���ô����Ϸ���������״̬���ȵĴ�С
	m_control_bar_height = DPI(30);
	m_margin = DPI(4);
	m_width_threshold = DPI(650);
	m_info_height = DPI(170);
	m_info_height2 = DPI(143);
	m_path_edit_height = DPI(32);
	m_progress_bar_height = DPI(25);
	m_spectral_size.cx = DPI(120);
	m_spectral_size.cy = DPI(90);

	m_set_path_button.GetWindowRect(rect);
	m_min_width = rect.left + DPI(8);		//��������·������ť����xλ����Ϊ���ڵ���С����
	m_min_height = DPI(340);	//������С�߶�

	CRect rect1;
	m_time_static.GetWindowRect(rect1);
	m_time_width = rect1.Width();		//����ʱ��ؼ���ʼʱ�Ŀ���
	m_time_height = rect1.Height();

	//��ʼ����ʾ��Ϣ
	m_Mytip.Create(this, TTS_ALWAYSTIP);
	m_Mytip.AddTool(GetDlgItem(ID_SET_PATH), _T("ѡ��������Ź���·��"));
	//m_Mytip.AddTool(GetDlgItem(IDC_VOLUME_DOWN), _T("��С����"));
	//m_Mytip.AddTool(GetDlgItem(IDC_VOLUME_UP), _T("��������"));
	m_Mytip.AddTool(&m_time_static, _T("����ʱ��"));

	//Ϊ��ʾ����ʱ���static�ؼ�����SS_NOTIFY���ԣ������������ʾ
	DWORD dwStyle = m_time_static.GetStyle();
	::SetWindowLong(m_time_static.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	m_list_popup_menu.LoadMenu(IDR_POPUP_MENU);		//װ�ز����б��Ҽ��˵�
	m_popup_menu.LoadMenu(IDR_LYRIC_POPUP_MENU);	//װ�ظ���Ҽ��˵�

	CoInitialize(0);	//��ʼ��COM���������֧����������ʾ���Ⱥ�����ͼ��ť
	CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pTaskbar));	//����ITaskbarList3��ʵ��

	//���밴ťͼ��
	m_hPlayPauseIcon = AfxGetApp()->LoadIcon(IDI_PLAY_PAUSE);
	m_hPreviousIcon = AfxGetApp()->LoadIcon(IDI_PREVIOUS);
	m_hNextIcon = AfxGetApp()->LoadIcon(IDI_NEXT1);
	m_hPlayIcon = AfxGetApp()->LoadIcon(IDI_PLAY);
	m_hPauseIcon = AfxGetApp()->LoadIcon(IDI_PAUSE);

	//���밴ťСͼ�꣨16*16��
	m_hPreviousIcon_s = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PREVIOUS), IMAGE_ICON, DPI(16), DPI(16), LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_hNextIcon_s = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NEXT1), IMAGE_ICON, DPI(16), DPI(16), LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_hPlayIcon_s = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PLAY), IMAGE_ICON, DPI(16), DPI(16), LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_hPauseIcon_s = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PAUSE), IMAGE_ICON, DPI(16), DPI(16), LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_hStopIcon_s = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_STOP), IMAGE_ICON, DPI(16), DPI(16), LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

	//��ʼ������������ͼ�еİ�ť
	THUMBBUTTONMASK dwMask = THB_ICON | THB_TOOLTIP | THB_FLAGS;
	//��һ����ť
	m_thumbButton[0].dwMask = dwMask;
	m_thumbButton[0].iId = IDT_PREVIOUS;
	m_thumbButton[0].hIcon = m_hPreviousIcon;
	wcscpy_s(m_thumbButton[0].szTip, _T("��һ��"));
	m_thumbButton[0].dwFlags = THBF_ENABLED;
	//����/��ͣ��ť
	m_thumbButton[1].dwMask = dwMask;
	m_thumbButton[1].iId = IDT_PLAY_PAUSE;
	m_thumbButton[1].hIcon = m_hPlayIcon;
	wcscpy_s(m_thumbButton[1].szTip, _T("����"));
	m_thumbButton[1].dwFlags = THBF_ENABLED;
	//��һ����ť
	m_thumbButton[2].dwMask = dwMask;
	m_thumbButton[2].iId = IDT_NEXT;
	m_thumbButton[2].hIcon = m_hNextIcon;
	wcscpy_s(m_thumbButton[2].szTip, _T("��һ��"));
	m_thumbButton[2].dwFlags = THBF_ENABLED;

	//�����������ϵİ�ťͼ��
	m_play_pause_button.SetFont(this->GetFont());
	m_play_pause_button.SetImage(m_hPlayIcon_s, FALSE);
	m_play_pause_button.SetWindowText(_T("����(&P)"));

	m_previous_button.SetFont(this->GetFont());
	m_previous_button.SetImage(m_hPreviousIcon_s);

	m_next_button.SetFont(this->GetFont());
	m_next_button.SetImage(m_hNextIcon_s);

	m_stop_button.SetFont(this->GetFont());
	m_stop_button.SetImage(m_hStopIcon_s);

	//��ʼ��������
	m_progress_bar.SetRange(1000);		//���ý������ķ�Χ
	m_progress_bar.SetProgressBarHeight(DPI(5));	//���ý������ĸ߶�
	m_progress_bar.SetColor(theApp.m_theme_color);			//���ý���������ɫ
	m_progress_bar.GetWindowRect(rect);
	m_progress_bar_left_pos = rect.left;		//�ÿؼ���ʼ��λ����Ϊ��ͨ����ģʽ�½������ؼ�����λ��

	//ע��ȫ���ȼ�
	//RegisterHotKey(m_hWnd, HK_PLAY_PAUSE, 0, VK_MEDIA_PLAY_PAUSE);
	RegisterHotKey(m_hWnd, HK_PLAY_PAUSE, MOD_CONTROL, VK_MEDIA_PLAY_PAUSE);		//ע��Ctrl+��ý�岥��/��ͣ��Ϊȫ���ȼ�
	RegisterHotKey(m_hWnd, HK_PLAY_PAUSE2, MOD_CONTROL | MOD_SHIFT, VK_F5);		//ע��Ctrl+Shift+F5Ϊȫ�ֲ���/��ͣ��
	RegisterHotKey(m_hWnd, HK_PREVIOUS, MOD_CONTROL | MOD_SHIFT, VK_LEFT);		//ע��Ctrl+Shift+��Ϊȫ����һ����
	RegisterHotKey(m_hWnd, HK_NEXT, MOD_CONTROL | MOD_SHIFT, VK_RIGHT);			//ע��Ctrl+Shift+��Ϊȫ����һ����
	RegisterHotKey(m_hWnd, HK_VOLUME_UP, MOD_CONTROL | MOD_SHIFT, VK_UP);		//ע��Ctrl+Shift+��Ϊȫ�������Ӽ�
	RegisterHotKey(m_hWnd, HK_VOLUME_DOWN, MOD_CONTROL | MOD_SHIFT, VK_DOWN);			//ע��Ctrl+Shift+��Ϊȫ����������

	//���ý������ɫ
	m_text_color = CColorConvert::ConvertToItemColor(theApp.m_theme_color);
	m_text_back_color = CColorConvert::ConvertToBackColor(theApp.m_theme_color);
	m_lyric_back_color = CColorConvert::ConvertToLightColor(theApp.m_theme_color);

	//��ʼ�����ҶԻ����е�����
	m_findDlg.LoadConfig();

	//��ȡCortana�ľ��
	GetCortanaHandle();

	//��ʼ����ͼ����
	m_pDC = GetDC();
	m_draw.Create(m_pDC, this);

	//������Cortana������ʾ��ʵ�����
	m_cortana_font.CreatePointFont(110, _T("΢���ź�"));

	//���ö�ʱ��
	SetTimer(TIMER_ID, TIMER_ELAPSE, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMusicPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == IDM_MINIMODE)
	{
		OnMiniMode();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMusicPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	//DrawInfo();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMusicPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMusicPlayerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	m_show_volume_adj = false;
	if (nType != SIZE_MINIMIZED)
	{
		if (m_pDC != NULL)
		{
			DrawInfo(true);
			CRect redraw_rect{m_draw_rect};
			if((cx < m_width_threshold)!= m_narrow_mode)	//�����խ����ģʽ����ͨģʽ֮��������л������ػ�ͻ���
			{
				Invalidate(FALSE);
				m_time_static.Invalidate(FALSE);
			}
			else if (!m_narrow_mode)	//����ͨ����ģʽ��
			{
				if (cx < m_client_width)	//���������ȱ�խ��
				{
					//���½���ͼ�����Ҳ�����ľ����������Ϊ�Ի��򱳾�ɫ
					redraw_rect.left = cx / 2 - 3 * m_margin;
					redraw_rect.right = m_client_width / 2 + m_margin;
					m_pDC->FillSolidRect(redraw_rect, GetSysColor(COLOR_BTNFACE));
				}
				else if (cy < m_client_height)	//�������߶ȱ�С��
				{
					//���½���ͼ�����·�����ľ����������Ϊ�Ի��򱳾�ɫ
					redraw_rect.top = cy - 2 * m_margin;
					redraw_rect.bottom = cy;
					m_pDC->FillSolidRect(redraw_rect, GetSysColor(COLOR_BTNFACE));
				}
			}
			else if (m_narrow_mode && cx < m_client_width)	//��խ����ģʽ�£�������ȱ�խ��
			{
				//���½���ͼ�����Ҳ�����ľ����������Ϊ�Ի��򱳾�ɫ
				redraw_rect.left = cx - 2 * m_margin;
				redraw_rect.right = cx;
				m_pDC->FillSolidRect(redraw_rect, GetSysColor(COLOR_BTNFACE));
			}
		}
		m_client_width = cx;
		m_client_height = cy;
		if (m_width_threshold != 0)
		{
			m_narrow_mode = (cx < m_width_threshold);
		}
		if (m_playlist_list.m_hWnd && theApp.m_dpi)
		{
			SetPlaylistSize(cx, cy);
		}
		if (m_progress_bar.m_hWnd && theApp.m_dpi)
		{
			SetPorgressBarSize(cx, cy);
		}
		SetThumbnailClipArea();

		if (nType != SIZE_MAXIMIZED)
		{
			CRect rect;
			GetWindowRect(&rect);
			m_window_width = rect.Width();
			m_window_height = rect.Height();
		}
	}

	// TODO: �ڴ˴�������Ϣ�����������
}


void CMusicPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	if (m_first_start)
	{
		//��if���ֻ�ڶ�ʱ����1�δ���ʱ��ִ��
		m_first_start = false;
		//��������������ͼ���ڰ�ť
		m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);

		CRect rect;
		GetClientRect(rect);
		m_client_width = rect.Width();
		m_client_height = rect.Height();
		SetPlaylistSize(rect.Width(), rect.Height());		//���������б��Ĵ�С��λ��
		m_path_static.Invalidate();
		SetPorgressBarSize(rect.Width(), rect.Height());		//�����������ڴ����еĴ�С��λ��
		SetThumbnailClipArea();

		if (m_cmdLine.empty())		//�ж��Ƿ���ͨ�������д��ļ�
		{
			theApp.m_player.Create();
		}
		else
		{
			vector<wstring> files;
			wstring path = CCommon::DisposeCmdLine(m_cmdLine, files);
			if (!path.empty())
				theApp.m_player.Create(path);
			else
				theApp.m_player.Create(files);
			//MessageBox(m_cmdLine.c_str(), NULL, MB_ICONINFORMATION);
		}
		DrawInfo();

		theApp.m_player.MusicControl(Command::OPEN);
		theApp.m_player.MusicControl(Command::SEEK);
		theApp.m_player.GetBASSError();
		SetPorgressBarSize(rect.Width(), rect.Height());		//���µ����������ڴ����еĴ�С��λ�ã���Ҫ���ݸ�����ʱ��������ʾʱ��ؼ��Ŀ��ȣ�
		ShowTime();
		m_progress_bar.SetSongLength(theApp.m_player.GetSongLength());

		if(!m_cmdLine.empty())
			theApp.m_player.MusicControl(Command::PLAY);	//����ļ���ͨ�������д򿪵ģ���򿪺�ֱ�Ӳ���

		sec_temp = theApp.m_player.GetCurrentSecond();
		UpdatePlayPauseButton();
		//SetForegroundWindow();
		//ShowPlayList();
	}

	m_timer_count++;

	UpdateTaskBarProgress();
	UpdateProgress();

	theApp.m_player.GetBASSError();
	if (m_miniModeDlg.m_hWnd == NULL)
		DrawInfo();			//���ƽ����ϵ���Ϣ�������ʾ������ģʽ���򲻻��ƽ�����Ϣ��
	if (theApp.m_player.IsPlaying())
	{
		theApp.m_player.GetBASSCurrentPosition();

		sec_current = theApp.m_player.GetCurrentSecond();
		if (sec_temp != sec_current)		//��ȡ����ʱ����������������������ˢ��һ��ʱ��
		{
			sec_temp = sec_current;
			ShowTime();
		}
		theApp.m_player.GetBASSSpectral();

		//��Cortana����������ʾ���
		if (m_show_lyric_in_cortana)
		{
			if (!theApp.m_player.m_Lyrics.IsEmpty())		//�и��ʱ��ʾ���
			{
				Time time{ theApp.m_player.GetCurrentPosition() };
				wstring lyric_text = theApp.m_player.m_Lyrics.GetLyric(time, 0);
				int progress = theApp.m_player.m_Lyrics.GetLyricProgress(time);
				if (lyric_text.empty()) lyric_text = DEFAULT_LYRIC_TEXT;
				DrawCortanaText(lyric_text.c_str(), progress);
			}
			else
			{
				//û�и��ʱ��Cortana���������Թ����ķ�ʽ��ʾ��ǰ���Ÿ������ļ���
				static int index{};
				static wstring song_name{};
				//�����ǰ���ŵĸ��������仯��DrawCortanaText�����ĵ�2����Ϊtrue�������ù���λ��
				if (index != theApp.m_player.GetIndex() || song_name != theApp.m_player.GetFileName())
				{
					DrawCortanaText((L"���ڲ��ţ�" + theApp.m_player.GetFileName()).c_str(), true);
					index = theApp.m_player.GetIndex();
					song_name = theApp.m_player.GetFileName();
				}
				else
				{
					DrawCortanaText((L"���ڲ��ţ�" + theApp.m_player.GetFileName()).c_str(), false);
				}
			}
		}
	}
	//if (theApp.m_player.SongIsOver() && (!m_stop_when_error || !theApp.m_player.IsError()))	//��ǰ��Ŀ���������û�г��ִ���ʱ�Ų�����һ��
	if (theApp.m_player.SongIsOver() || (!m_stop_when_error && theApp.m_player.IsError()))	//��ǰ��Ŀ���������û�г��ִ���ʱ�Ų�����һ��
	{
		if ((m_pLyricEdit != nullptr && m_pLyricEdit->m_dlg_exist) || !theApp.m_player.PlayTrack(NEXT))		//��ǰ���ڱ༭��ʣ���˳�򲥷�ģʽ���б��еĸ����������ʱ��PlayTrack�����᷵��false���������굱ǰ������ֹͣ����
		{
			theApp.m_player.MusicControl(Command::STOP);		//ֹͣ����
			ShowTime();
			if (m_show_lyric_in_cortana)
				ResetCortanaText();
		}
		SwitchTrack();
		UpdatePlayPauseButton();
	}
	if (theApp.m_player.IsPlaying() && (m_stop_when_error && theApp.m_player.IsError()))
	{
		theApp.m_player.MusicControl(Command::PAUSE);
		UpdatePlayPauseButton();
	}

	//��������������ɫ����ϵͳʱ�������ǰ�������������ϣ���ʱ��ÿ����10�λ�ȡһ�δ��ڱ���������ɫ
	if (m_theme_color_follow_system && m_timer_count % 10 == 0 && ::GetForegroundWindow() == m_hWnd)
	{
		HDC hDC = ::GetDC(NULL);
		CRect rect;
		GetWindowRect(rect);
		if (rect.left < 0) rect.left = 0;
		if (rect.top < 0) rect.top = 0;
		//��ȡ�������Ͻǵ�����12�����ص����ɫ����Ϊ���ڱ�Ե����ɫ��
		COLORREF color;
		//���ϵͳ��Win10��ȡ�������Ե���ص���ɫ����Ϊ��win10���óɲ���ʾ��������ɫʱ��ֻ�д��ڱ߿��������һȦ������������ɫ
		//���ϵͳ��Win8/8.1����ȡ���ڱ�Եס��2�����ص���ɫ����Ϊwin8���ڱ��������ǵ�ǰ����ɫ�����߿���������ɫ������ɫҪ�
		if (theApp.m_is_windows10)
			color = ::GetPixel(hDC, rect.TopLeft().x + 12, rect.TopLeft().y);
		else
			color = ::GetPixel(hDC, rect.TopLeft().x + 12, rect.TopLeft().y + 2);
		if (theApp.m_theme_color != color && color != RGB(255,255,255))	//��ǰ����ɫ���˵�ʱ��������������ɫ������ȷ����ȡ������ɫ���Ǵ���ɫ
		{
			theApp.m_theme_color = color;
			m_progress_bar.SetColor(theApp.m_theme_color);		//���ý�������ɫ
			m_progress_bar.Invalidate();
			m_time_static.Invalidate();
			m_text_color = CColorConvert::ConvertToItemColor(theApp.m_theme_color);
			m_text_back_color = CColorConvert::ConvertToBackColor(theApp.m_theme_color);
			m_lyric_back_color = CColorConvert::ConvertToLightColor(theApp.m_theme_color);
			SetPlayListColor();
		}
	}

	//if (nIDEvent == TIMER_ID_EXIT)
	//{
	//	//5�붨ʱ����ʱ�䵽�ˣ�ǿ�ƽ�������
	//	HANDLE hself = GetCurrentProcess();
	//	TerminateProcess(hself, 0);
	//}

	CDialog::OnTimer(nIDEvent);
}


void CMusicPlayerDlg::OnPlayPause()
{
	// TODO: �ڴ�����������������
	theApp.m_player.MusicControl(Command::PLAY_PAUSE);
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnStop()
{
	// TODO: �ڴ�����������������
	theApp.m_player.MusicControl(Command::STOP);
	UpdatePlayPauseButton();
	ShowTime();
}


void CMusicPlayerDlg::OnPrevious()
{
	// TODO: �ڴ�����������������
	theApp.m_player.PlayTrack(PREVIOUS);
	SwitchTrack();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnNext()
{
	// TODO: �ڴ�����������������
	theApp.m_player.PlayTrack(NEXT);
	SwitchTrack();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnRew()
{
	// TODO: �ڴ�����������������
	theApp.m_player.MusicControl(Command::REW);
	UpdateTaskBarProgress();
	ShowTime();
}


void CMusicPlayerDlg::OnFF()
{
	// TODO: �ڴ�����������������
	theApp.m_player.MusicControl(Command::FF);
	UpdateTaskBarProgress();
	ShowTime();
}


void CMusicPlayerDlg::OnSetPath()
{
	// TODO: �ڴ�����������������
	CSetPathDlg setPathDlg(theApp.m_player.GetRecentPath(), theApp.m_player.GetCurrentPath());
	if (setPathDlg.DoModal() == IDOK && setPathDlg.SelectValid())
	{
		theApp.m_player.SetPath(setPathDlg.GetSelPath(), setPathDlg.GetTrack(), setPathDlg.GetPosition(), setPathDlg.GetSortMode());
		UpdatePlayPauseButton();
		SetPorgressBarSize();
		ShowTime();
		DrawInfo(true);
		m_find_result.clear();		//����·����������ҽ��
	}
	theApp.m_player.SaveRecentPath();
}


void CMusicPlayerDlg::OnFind()
{
	// TODO: �ڴ�����������������
	if (m_findDlg.DoModal() == IDOK)
	{
		theApp.m_player.PlayTrack(m_findDlg.GetSelectedTrack());
		SwitchTrack();
		UpdatePlayPauseButton();
	}
}


void CMusicPlayerDlg::OnExplorePath()
{
	// TODO: �ڴ�����������������
	theApp.m_player.ExplorePath();
}


BOOL CMusicPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ�����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		//����Ctrl��ʱ
		if (GetKeyState(VK_CONTROL) & 0x80)
		{
			if (pMsg->wParam == 'O')		//���ð�Ctr+O���ļ�
			{
				OnFileOpen();
				return TRUE;
			}
			if (pMsg->wParam == 'D')		//���ð�Ctr+D���ļ���
			{
				OnFileOpenFolder();
				return TRUE;
			}
			if (pMsg->wParam == 'E')		//���ð�Ctr+E����ļ�
			{
				OnExplorePath();
				return TRUE;
			}
			if (pMsg->wParam == 'F')		//��Ctr+F�������ļ�
			{
				OnFind();
				return TRUE;
			}
			if (pMsg->wParam == 'T')		//��Ctr+T������·��
			{
				OnSetPath();
				return TRUE;
			}
			if (pMsg->wParam == 'I')		//���ð�Ctr+I������
			{
				OnOptionSettings();
				return TRUE;
			}
			if (pMsg->wParam == 'X')		//���ð�Ctr+X�˳�
			{
				OnCancel();
				return TRUE;
			}
			if (pMsg->wParam == 'R')		//���ð�Ctr+R���³�ʼ��BASS��Ƶ��
			{
				OnReIniBass();
				return TRUE;
			}
			if (pMsg->wParam == 'M')		//���ð�Ctr+M��������ģʽ
			{
				OnMiniMode();
				return TRUE;
			}
			if (pMsg->wParam == VK_PRIOR)		//���ð�Ctr+PgUp�����ǰ0.5��
			{
				OnLyricForward();
				return TRUE;
			}
			if (pMsg->wParam == VK_NEXT)		//���ð�Ctr+PgDn����Ӻ�0.5��
			{
				OnLyricDelay();
				return TRUE;
			}
			if (pMsg->wParam == 'S')		//���ð�Ctr+S�����ʸ���
			{
				if (!theApp.m_player.m_Lyrics.IsEmpty() && theApp.m_player.m_Lyrics.IsModified())
					OnSaveModifiedLyric();
				return TRUE;
			}
		}
		else
		{
			if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)		//���ΰ��س�����ESC���˳�
			{
				return TRUE;
			}
			if (pMsg->wParam == VK_SPACE || pMsg->wParam == 'P' || pMsg->wParam == VK_MEDIA_PLAY_PAUSE)		//���ո��/P������/��ͣ
			{
				OnPlayPause();
				return TRUE;
			}
			if (pMsg->wParam == 'S')	//��S��ֹͣ
			{
				OnStop();
				return TRUE;
			}
			if (pMsg->wParam == 'V')	//��V����һ��
			{
				OnPrevious();
				return TRUE;
			}
			if (pMsg->wParam == 'N')	//��N����һ��
			{
				OnNext();
				return TRUE;
			}
			if (pMsg->wParam == VK_LEFT)	//��������¿���5��
			{
				OnRew();
				return TRUE;
			}
			if (pMsg->wParam == VK_RIGHT)	//���ҷ�����¿��5��
			{
				OnFF();
				return TRUE;
			}
			if (pMsg->wParam == VK_UP)	//���Ϸ������������
			{
				theApp.m_player.MusicControl(Command::VOLUME_UP);
				return TRUE;
			}
			if (pMsg->wParam == VK_DOWN)	//���·����������
			{
				theApp.m_player.MusicControl(Command::VOLUME_DOWN);
				return TRUE;
			}
			if (pMsg->wParam == 'M')	//��M������ѭ��ģʽ
			{
				theApp.m_player.SetRepeatMode();
				return TRUE;
			}
			//if (pMsg->wParam == 'T')	//��T������·��
			//{
			//	OnSetPath();
			//	return TRUE;
			//}
			//if (pMsg->wParam == 'F')	//��F�������ļ�
			//{
			//	OnFind();
			//	return TRUE;
			//}
			//if (pMsg->wParam == 0x00DB || (pMsg->wParam == 0x00E5 && pMsg->lParam == 0x001A0001))	//����[�����򡰡����������б���ǰ��ҳ
			//{
			//	theApp.m_player.SwitchPlaylist(PREVIOUS);
			//	return TRUE;
			//}
			//if (pMsg->wParam == 0x00DD || (pMsg->wParam == 0x00E5 && pMsg->lParam == 0x001B0001))	//����]�����򡰡����������б����ҳ
			//{
			//	theApp.m_player.SwitchPlaylist(NEXT);
			//	return TRUE;
			//}
		}
	}

	if (pMsg->message == WM_MOUSEMOVE)
		m_Mytip.RelayEvent(pMsg);


	return CDialog::PreTranslateMessage(pMsg);
}


void CMusicPlayerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴�������Ϣ�����������
	//�˳�ʱ��������
	theApp.m_player.SaveConfig();
	SaveConfig();
	m_findDlg.SaveConfig();
	//���ȫ���ȼ�
	UnregisterHotKey(m_hWnd, HK_PLAY_PAUSE);
	UnregisterHotKey(m_hWnd, HK_PLAY_PAUSE2);
	UnregisterHotKey(m_hWnd, HK_PREVIOUS);
	UnregisterHotKey(m_hWnd, HK_NEXT);
	UnregisterHotKey(m_hWnd, HK_VOLUME_UP);
	UnregisterHotKey(m_hWnd, HK_VOLUME_DOWN);

	//�˳�ʱ�ָ�Cortana��Ĭ���ı�
	ResetCortanaText();

	//SetTimer(TIMER_ID_EXIT, 5000, NULL);
	//FreeConsole();
}


void CMusicPlayerDlg::OnAppAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CMusicPlayerDlg::OnFileOpen()
{
	// TODO: �ڴ�����������������
	vector<wstring> files;	//����򿪵Ķ���ļ�·��
	//���ù�����
	LPCTSTR szFilter = _T("��Ƶ�ļ�(*.mp3;*.wma;*.wav;*.ogg;*.flac;*.m4a)|*.mp3;*.wma;*.wav;*.ogg;*.flac;*.m4a|�����ļ�(*.*)|*.*||");
	//������ļ��Ի���
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, szFilter, this);
	//���ñ����ļ������ַ�����Ĵ�СΪ128kB�������ƽ��һ���ļ�������Ϊ32�ֽڼ��㣬�����Դ򿪴�Լ4096���ļ���
	fileDlg.m_ofn.nMaxFile = 128 * 1024;
	LPTSTR ch = new TCHAR[fileDlg.m_ofn.nMaxFile];
	fileDlg.m_ofn.lpstrFile = ch;
	//���ڴ������
	ZeroMemory(fileDlg.m_ofn.lpstrFile, sizeof(TCHAR) * fileDlg.m_ofn.nMaxFile);
	//��ʾ���ļ��Ի���
	if (IDOK == fileDlg.DoModal())
	{
		POSITION posFile = fileDlg.GetStartPosition();
		while (posFile != NULL)
		{
			files.push_back(fileDlg.GetNextPathName(posFile).GetString());
		}
		theApp.m_player.OpenFiles(files);
		//ShowPlayList();
		UpdatePlayPauseButton();
		SetPorgressBarSize();
		DrawInfo(true);
	}
}


void CMusicPlayerDlg::OnFileOpenFolder()
{
	// TODO: �ڴ�����������������
	
	CFolderPickerDialog folderPickerDlg(theApp.m_player.GetCurrentPath().c_str());
	if (folderPickerDlg.DoModal() == IDOK)
	{
		theApp.m_player.OpenFolder(wstring(folderPickerDlg.GetPathName()));
		//ShowPlayList();
		UpdatePlayPauseButton();
		SetPorgressBarSize();
		DrawInfo(true);
	}
}


void CMusicPlayerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	vector<wstring> files;	//�����Ϸŵ����ڵĶ���ļ�·��
	TCHAR file_path[MAX_PATH];
	int drop_count = DragQueryFile(hDropInfo, -1, NULL, 0);		//ȡ�ñ��϶��ļ�����Ŀ
	//��ȡ��1���ļ�
	DragQueryFile(hDropInfo, 0, file_path, MAX_PATH);
	wstring file_path_wcs{ file_path };
	if (file_path_wcs.size() > 4 && file_path_wcs[file_path_wcs.size() - 4] != L'.' && file_path_wcs[file_path_wcs.size() - 5] != L'.')
	{
		//�����ȡ�ĵ�һ���ļ��ĵ�����3���͵�����4���ַ������ǡ�.����˵���ⲻ��һ���ļ�������һ���ļ���
		file_path_wcs.push_back(L'\\');
		theApp.m_player.OpenFolder(file_path_wcs);
	}
	else
	{
		for (int i{}; i < drop_count; i++)
		{
			DragQueryFile(hDropInfo, i, file_path, MAX_PATH);	//��ȡ��i���ļ�
			if (CAudioCommon::FileIsAudio(wstring(file_path)))
				files.push_back(file_path);
		}
		theApp.m_player.OpenFiles(files, false);
	}
	//ShowPlayList();
	UpdatePlayPauseButton();
	SetPorgressBarSize();
	DrawInfo(true);

	CDialog::OnDropFiles(hDropInfo);
}


void CMusicPlayerDlg::OnInitMenu(CMenu* pMenu)
{
	CDialog::OnInitMenu(pMenu);
	//����ѭ��ģʽ�˵��ĵ�ѡ���
	RepeatMode repeat_mode{ theApp.m_player.GetRepeatMode() };
	switch (repeat_mode)
	{
	case RM_PLAY_ORDER: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_PLAY_ORDER, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_PLAY_SHUFFLE: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_PLAY_SHUFFLE, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_LOOP_PLAYLIST: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_LOOP_PLAYLIST, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_LOOP_TRACK: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_LOOP_TRACK, MF_BYCOMMAND | MF_CHECKED); break;
	default: break;
	}

	//�����Ҽ��˵�ʱ�����û��ѡ�в����б��е���Ŀ��������Ҽ��˵��С����š��������б���ɾ�����������ԡ������Ӵ���ɾ������Ŀ��
	if (m_item_selected < 0 || m_item_selected >= theApp.m_player.GetSongNum())
	{
		pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | MF_ENABLED);
	}

	//�򿪲˵�ʱ����������б���û�и�������������˵����Ҽ��˵��еġ����ļ�λ�á���Ŀ
	if (theApp.m_player.GetSongNum() == 0)
	{
		pMenu->EnableMenuItem(ID_EXPLORE_PATH, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EXPLORE_TRACK, MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_EXPLORE_PATH, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_EXPLORE_TRACK, MF_BYCOMMAND | MF_ENABLED);
	}

	//���ò����б��˵�������ʽ�ĵ�ѡ���
	switch (theApp.m_player.m_sort_mode)
	{
	case SM_FILE: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_FILE, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_TITLE: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_TITLE, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_ARTIST: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_ARTIST, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_ALBUM: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_ALBUM, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_TRACK: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_TRACK, MF_BYCOMMAND | MF_CHECKED); break;
	}

	//���ò����б��˵��С������б���ʾ��ʽ���ĵ�ѡ���
	switch (m_display_format)
	{
	case DF_FILE_NAME: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_FILE_NAME, MF_BYCOMMAND | MF_CHECKED); break;
	case DF_TITLE: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_TITLE, MF_BYCOMMAND | MF_CHECKED); break;
	case DF_ARTIST_TITLE: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_ARTIST_TITLE, MF_BYCOMMAND | MF_CHECKED); break;
	case DF_TITLE_ARTIST: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_TITLE_ARTIST, MF_BYCOMMAND | MF_CHECKED); break;
	}

	//���ò����б��Ҽ��˵���Ĭ�ϲ˵���
	pMenu->SetDefaultItem(ID_PLAY_ITEM);

	//���ݸ���Ƿ�����������û���ò˵���
	//pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | (!theApp.m_player.m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_COPY_CURRENT_LYRIC, MF_BYCOMMAND | (!theApp.m_player.m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_COPY_ALL_LYRIC, MF_BYCOMMAND | (!theApp.m_player.m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
	//pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | (!theApp.m_player.m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_LYRIC_FORWARD, MF_BYCOMMAND | (!theApp.m_player.m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_LYRIC_DELAY, MF_BYCOMMAND | (!theApp.m_player.m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_SAVE_MODIFIED_LYRIC, MF_BYCOMMAND | ((!theApp.m_player.m_Lyrics.IsEmpty() && theApp.m_player.m_Lyrics.IsModified()) ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_DELETE_LYRIC, MF_BYCOMMAND | (!theApp.m_player.m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));

	// TODO: �ڴ˴�������Ϣ�����������
}


void CMusicPlayerDlg::OnPlayOrder()
{
	// TODO: �ڴ�����������������
	theApp.m_player.SetRepeatMode(RM_PLAY_ORDER);		//����˳�򲥷�
}


void CMusicPlayerDlg::OnPlayShuffle()
{
	// TODO: �ڴ�����������������
	theApp.m_player.SetRepeatMode(RM_PLAY_SHUFFLE);		//�����������
}


void CMusicPlayerDlg::OnLoopPlaylist()
{
	// TODO: �ڴ�����������������
	theApp.m_player.SetRepeatMode(RM_LOOP_PLAYLIST);		//�����б�ѭ��
}


void CMusicPlayerDlg::OnLoopTrack()
{
	// TODO: �ڴ�����������������
	theApp.m_player.SetRepeatMode(RM_LOOP_TRACK);		//���õ���ѭ��
}


BOOL CMusicPlayerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	if (zDelta > 0)
	{
		theApp.m_player.MusicControl(Command::VOLUME_UP);
	}
	if (zDelta < 0)
	{
		theApp.m_player.MusicControl(Command::VOLUME_DOWN);
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


//void CMusicPlayerDlg::OnBnClickedVolumeUp()
//{
//	// TODO: �ڴ����ӿؼ�֪ͨ�����������
//	theApp.m_player.MusicControl(Command::VOLUME_UP, 5);
//	//theApp.m_player.ShowVolume();
//}


//void CMusicPlayerDlg::OnBnClickedVolumeDown()
//{
//	// TODO: �ڴ����ӿؼ�֪ͨ�����������
//	theApp.m_player.MusicControl(Command::VOLUME_DOWN, 5);
//	//theApp.m_player.ShowVolume();
//}


void CMusicPlayerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	//���ƴ�����С��С
	lpMMI->ptMinTrackSize.x = m_min_width;		//������С����
	lpMMI->ptMinTrackSize.y = m_min_height;		//������С�߶�

	CDialog::OnGetMinMaxInfo(lpMMI);
}


void CMusicPlayerDlg::OnNMDblclkPlaylistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	int row = pNMItemActivate->iItem;
	theApp.m_player.PlayTrack(row);
	SwitchTrack();
	UpdatePlayPauseButton();

	*pResult = 0;
}


//void CMusicPlayerDlg::OnRefreshPlaylist()
//{
//	// TODO: �ڴ�����������������
//	ShowPlayList();
//}


void CMusicPlayerDlg::OnOptionSettings()
{
	// TODO: �ڴ�����������������
	COptionsDlg optionDlg;
	//��ʼ���Ի����б�����ֵ
	optionDlg.m_tab_selected = m_tab_selected;

	optionDlg.m_tab1_dlg.m_stop_when_error = m_stop_when_error;
	optionDlg.m_tab1_dlg.m_show_taskbar_progress = m_show_taskbar_progress;
	optionDlg.m_tab1_dlg.m_karaoke_disp = theApp.m_player.m_lyric_karaoke_disp;
	optionDlg.m_tab1_dlg.m_lyric_fuzzy_match = theApp.m_player.m_lyric_fuzzy_match;
	optionDlg.m_tab1_dlg.m_show_lyric_in_cortana = m_show_lyric_in_cortana;
	optionDlg.m_tab1_dlg.m_save_lyric_in_offset = m_save_lyric_in_offset;
	optionDlg.m_tab1_dlg.m_lyric_path = theApp.m_player.m_lyric_path;

	optionDlg.m_tab2_dlg.m_hMainWnd = m_hWnd;
	optionDlg.m_tab2_dlg.m_font = m_lyric_font_name;
	optionDlg.m_tab2_dlg.m_font_size = m_lyric_font_size;
	optionDlg.m_tab2_dlg.m_line_space = m_lyric_line_space;
	optionDlg.m_tab2_dlg.m_transparency = m_transparency;
	optionDlg.m_tab2_dlg.m_theme_color = theApp.m_theme_color;
	optionDlg.m_tab2_dlg.m_theme_color_follow_system = m_theme_color_follow_system;

	if (optionDlg.DoModal() == IDOK)
	{
		//��ȡѡ�����öԻ����е���������
		m_stop_when_error = optionDlg.m_tab1_dlg.m_stop_when_error;
		m_show_taskbar_progress = optionDlg.m_tab1_dlg.m_show_taskbar_progress;
		theApp.m_player.m_lyric_karaoke_disp = optionDlg.m_tab1_dlg.m_karaoke_disp;
		theApp.m_player.m_lyric_fuzzy_match = optionDlg.m_tab1_dlg.m_lyric_fuzzy_match;
		if (m_show_lyric_in_cortana == true && optionDlg.m_tab1_dlg.m_show_lyric_in_cortana == false)	//�����ѡ���йر��ˡ���Cortana����������ʾ��ʡ���ѡ�������Cortana��������ı�
			ResetCortanaText();
		m_show_lyric_in_cortana = optionDlg.m_tab1_dlg.m_show_lyric_in_cortana;
		m_save_lyric_in_offset = optionDlg.m_tab1_dlg.m_save_lyric_in_offset;
		theApp.m_player.m_lyric_path = optionDlg.m_tab1_dlg.m_lyric_path;

		m_transparency = optionDlg.m_tab2_dlg.m_transparency;
		theApp.m_theme_color = optionDlg.m_tab2_dlg.m_theme_color;
		m_text_color = CColorConvert::ConvertToItemColor(theApp.m_theme_color);
		m_text_back_color = CColorConvert::ConvertToBackColor(theApp.m_theme_color);
		m_lyric_back_color = CColorConvert::ConvertToLightColor(theApp.m_theme_color);
		m_progress_bar.SetColor(theApp.m_theme_color);		//���ý�������ɫ
		m_progress_bar.Invalidate();
		m_time_static.Invalidate();
		SetPlayListColor();
		m_theme_color_follow_system = optionDlg.m_tab2_dlg.m_theme_color_follow_system;
		m_lyric_font_name = optionDlg.m_tab2_dlg.m_font;
		m_lyric_font_size = optionDlg.m_tab2_dlg.m_font_size;
		m_lyric_line_space = optionDlg.m_tab2_dlg.m_line_space;
		if (optionDlg.m_tab2_dlg.FontChanged())
		{
			//���m_font�Ѿ�������һ��������Դ�������ͷ���
			if (m_lyric_font.m_hObject)
				m_lyric_font.DeleteObject();
			m_lyric_font.CreatePointFont(m_lyric_font_size * 10, m_lyric_font_name.c_str());
		}
		theApp.m_player.SaveConfig();		//������д�뵽ini�ļ�
	}
	else
	{
		SetTransparency();		//��������ȡ��������Ҫ�������ô���͸����
	}

	m_tab_selected = optionDlg.m_tab_selected;
}


void CMusicPlayerDlg::OnReloadPlaylist()
{
	// TODO: �ڴ�����������������
	theApp.m_player.ReloadPlaylist();
	//ShowPlayList();
	UpdatePlayPauseButton();
	ShowTime();
}


void CMusicPlayerDlg::OnNMRClickPlaylistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_item_selected = pNMItemActivate->iItem;	//��ȡ���ѡ�е���Ŀ

	CMenu* pContextMenu = m_list_popup_menu.GetSubMenu(0); //��ȡ��һ�������˵�
	CPoint point;			//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����

	if (m_item_selected >= 0 && m_item_selected < theApp.m_player.GetSongNum())
	{
		CRect item_rect;
		m_playlist_list.GetItemRect(m_item_selected, item_rect, LVIR_BOUNDS);		//��ȡѡ����Ŀ�ľ��������Բ����б��ؼ����Ͻ�Ϊԭ�㣩
		CRect playlist_rect;
		m_playlist_list.GetWindowRect(playlist_rect);		//��ȡ�����б��ؼ��ľ�����������Ļ���Ͻ�Ϊԭ�㣩
		point.y = playlist_rect.top + item_rect.bottom;	//�������Ҫ������y����Ϊѡ����Ŀ���±߿�λ�ã���ֹ�Ҽ��˵���סѡ�е���Ŀ
	}

	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this); //��ָ��λ����ʾ�����˵�

	*pResult = 0;
}


void CMusicPlayerDlg::OnPlayItem()
{
	// TODO: �ڴ�����������������
	theApp.m_player.PlayTrack(m_item_selected);
	SwitchTrack();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnItemProperty()
{
	// TODO: �ڴ�����������������
	CPropertyDlg propertyDlg(theApp.m_player.GetPlayList());
	propertyDlg.m_path = theApp.m_player.GetCurrentPath();
	propertyDlg.m_index = m_item_selected;
	propertyDlg.m_song_num = theApp.m_player.GetSongNum();
	//propertyDlg.m_playing_index = theApp.m_player.GetIndex();
	//propertyDlg.m_lyric_name = theApp.m_player.GetLyricName();
	propertyDlg.DoModal();
}


void CMusicPlayerDlg::OnRemoveFromPlaylist()
{
	// TODO: �ڴ�����������������
	theApp.m_player.RemoveSong(m_item_selected);
	ShowPlayList();
	UpdatePlayPauseButton();
}


//void CMusicPlayerDlg::OnClearPlaylist()
//{
//	// TODO: �ڴ�����������������
//	theApp.m_player.ClearPlaylist();
//	ShowPlayList();
//	UpdatePlayPauseButton();
//}


void CMusicPlayerDlg::OnExploreTrack()
{
	// TODO: �ڴ�����������������
	theApp.m_player.ExplorePath(m_item_selected);
}


BOOL CMusicPlayerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ�����ר�ô����/����û���
	//��Ӧ����������ͼ��ť
	WORD command = LOWORD(wParam);
	switch(command)
	{
	case IDT_PLAY_PAUSE: OnPlayPause(); break;
	case IDT_PREVIOUS: OnPrevious(); break;
	case IDT_NEXT: OnNext(); break;
	}

	return CDialog::OnCommand(wParam, lParam);
}


void CMusicPlayerDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	//��Ӧȫ�ֿ�ݼ�
	switch (nHotKeyId)
	{
	case HK_PLAY_PAUSE: case HK_PLAY_PAUSE2: OnPlayPause(); break;
	case HK_PREVIOUS: OnPrevious(); break;
	case HK_NEXT: OnNext(); break;
	case HK_VOLUME_UP:
		theApp.m_player.MusicControl(Command::VOLUME_UP);
		break;
	case HK_VOLUME_DOWN:
		theApp.m_player.MusicControl(Command::VOLUME_DOWN);
		break;
	default: break;
	}

	CDialog::OnHotKey(nHotKeyId, nKey1, nKey2);
}



void CMusicPlayerDlg::OnStnClickedProgressStatic()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	int pos = m_progress_bar.GetPos();
	int song_pos = static_cast<__int64>(pos) * theApp.m_player.GetSongLength() / 1000;
	theApp.m_player.SeekTo(song_pos);
	ShowTime();
}


void CMusicPlayerDlg::OnReIniBass()
{
	// TODO: �ڴ�����������������
	theApp.m_player.ReIniBASS();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnSortByFile()
{
	// TODO: �ڴ�����������������
	theApp.m_player.m_sort_mode = SM_FILE;
	theApp.m_player.SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByTitle()
{
	// TODO: �ڴ�����������������
	theApp.m_player.m_sort_mode = SM_TITLE;
	theApp.m_player.SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByArtist()
{
	// TODO: �ڴ�����������������
	theApp.m_player.m_sort_mode = SM_ARTIST;
	theApp.m_player.SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByAlbum()
{
	// TODO: �ڴ�����������������
	theApp.m_player.m_sort_mode = SM_ALBUM;
	theApp.m_player.SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByTrack()
{
	// TODO: �ڴ�����������������
	theApp.m_player.m_sort_mode = SM_TRACK;
	theApp.m_player.SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnDeleteFromDisk()
{
	// TODO: �ڴ�����������������
	if (m_item_selected >= 0 && m_item_selected < theApp.m_player.GetSongNum())
	{
		if (m_item_selected == theApp.m_player.GetIndex())	//���ɾ�����ļ������ڲ��ŵ��ļ�����ɾ��ǰ�����ȹر��ļ�
			theApp.m_player.MusicControl(Command::CLOSE);
		wstring file_name{ theApp.m_player.GetCurrentPath() + theApp.m_player.GetPlayList()[m_item_selected].file_name };
		int rtn;
		rtn = CCommon::DeleteAFile(m_hWnd, file_name);
		if (rtn == 0)
		{
			//����ļ�ɾ���ɹ���ͬʱ�Ӳ����б����Ƴ�
			theApp.m_player.RemoveSong(m_item_selected);
			ShowPlayList();
			UpdatePlayPauseButton();
			DrawInfo(true);
		}
		else if (rtn == 1223)	//����ڵ����ĶԻ����е����ȡ�����򷵻�ֵΪ1223
		{
			if (m_item_selected == theApp.m_player.GetIndex())		//���ɾ�����ļ������ڲ��ŵ��ļ����ֵ���ˡ�ȡ�����������´򿪵�ǰ�ļ�
			{
				theApp.m_player.MusicControl(Command::OPEN);
				theApp.m_player.MusicControl(Command::SEEK);
				//theApp.m_player.Refresh();
				UpdatePlayPauseButton();
				DrawInfo(true);
			}
		}
		else
		{
			MessageBox(_T("�޷�ɾ���ļ���"), NULL, MB_ICONWARNING);
		}
	}
}


afx_msg LRESULT CMusicPlayerDlg::OnTaskbarcreated(WPARAM wParam, LPARAM lParam)
{
	//����Դ������������������������������ͼ���ڰ�ť
	m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);
	SetThumbnailClipArea();

	//��Դ������������Cortana�ľ���ᷢ���ı䣬��ʱҪ���»�ȡCortana�ľ��
	GetCortanaHandle();

	return 0;
}


void CMusicPlayerDlg::OnDispFileName()
{
	// TODO: �ڴ�����������������
	m_display_format = DF_FILE_NAME;
	ShowPlayList();
}


void CMusicPlayerDlg::OnDispTitle()
{
	// TODO: �ڴ�����������������
	m_display_format = DF_TITLE;
	ShowPlayList();
}


void CMusicPlayerDlg::OnDispArtistTitle()
{
	// TODO: �ڴ�����������������
	m_display_format = DF_ARTIST_TITLE;
	ShowPlayList();
}


void CMusicPlayerDlg::OnDispTitleArtist()
{
	// TODO: �ڴ�����������������
	m_display_format = DF_TITLE_ARTIST;
	ShowPlayList();
}


void CMusicPlayerDlg::OnMiniMode()
{
	// TODO: �ڴ�����������������
	if (m_miniModeDlg.m_hWnd != NULL)
		return;

	m_miniModeDlg.m_display_format = m_display_format;
	ShowWindow(SW_HIDE);
	if (m_miniModeDlg.DoModal() == IDCANCEL)
	{
		SendMessage(WM_COMMAND, ID_APP_EXIT);
	}
	else
	{
		ShowWindow(SW_SHOW);
		m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);	//������������������ͼ��ť
		SetThumbnailClipArea();		//������������������ͼ
		SetForegroundWindow();
		SwitchTrack();
	}
}


void CMusicPlayerDlg::OnBnClickedStop()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	SetFocus();
	OnStop();
}


void CMusicPlayerDlg::OnBnClickedPrevious()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	SetFocus();
	OnPrevious();
}


void CMusicPlayerDlg::OnBnClickedPlayPause()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	SetFocus();
	OnPlayPause();
}


void CMusicPlayerDlg::OnBnClickedNext()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	SetFocus();
	OnNext();
}


//void CMusicPlayerDlg::OnMove(int x, int y)
//{
//	CDialog::OnMove(x, y);
//
//	// TODO: �ڴ˴�������Ϣ�����������
//	SetMaskWindowPos();
//}


void CMusicPlayerDlg::OnReloadLyric()
{
	// TODO: �ڴ�����������������
	theApp.m_player.SearchLyrics();
	theApp.m_player.IniLyrics();
}


void CMusicPlayerDlg::OnSongInfo()
{
	// TODO: �ڴ�����������������
	CPropertyDlg propertyDlg(theApp.m_player.GetPlayList());
	propertyDlg.m_path = theApp.m_player.GetCurrentPath();
	propertyDlg.m_index = theApp.m_player.GetIndex();
	propertyDlg.m_song_num = theApp.m_player.GetSongNum();
	propertyDlg.DoModal();

}


void CMusicPlayerDlg::OnCopyCurrentLyric()
{
	// TODO: �ڴ�����������������
	if (CCommon::CopyStringToClipboard(theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 0)))
		MessageBox(_T("��ǰ����ѳɹ����Ƶ������塣"), NULL, MB_ICONINFORMATION);
	else
		MessageBox(_T("���Ƶ�������ʧ�ܣ�"), NULL, MB_ICONWARNING);
}


void CMusicPlayerDlg::OnCopyAllLyric()
{
	// TODO: �ڴ�����������������
	if (CCommon::CopyStringToClipboard(theApp.m_player.m_Lyrics.GetAllLyricText()))
		MessageBox(_T("�Ѹ���ȫ����ʵ������塣"), NULL, MB_ICONINFORMATION);
	else
		MessageBox(_T("���Ƶ�������ʧ�ܣ�"), NULL, MB_ICONWARNING);
}


void CMusicPlayerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	//˫����������������ģʽ
	if (!m_repetemode_rect.PtInRect(point) && !m_volume_rect.PtInRect(point) && !m_volume_up_rect.PtInRect(point) && !m_volume_down_rect.PtInRect(point))
		OnMiniMode();
	CDialog::OnLButtonDblClk(nFlags, point);
}


void CMusicPlayerDlg::OnLyricForward()
{
	// TODO: �ڴ�����������������
	//�����ǰ0.5��
	theApp.m_player.m_Lyrics.AdjustLyric(-500);

}


void CMusicPlayerDlg::OnLyricDelay()
{
	// TODO: �ڴ�����������������
	//����Ӻ�0.5��
	theApp.m_player.m_Lyrics.AdjustLyric(500);
}


void CMusicPlayerDlg::OnSaveModifiedLyric()
{
	// TODO: �ڴ�����������������
	if (m_save_lyric_in_offset)
		theApp.m_player.m_Lyrics.SaveLyric();
	else
		theApp.m_player.m_Lyrics.SaveLyric2();
}


void CMusicPlayerDlg::OnEditLyric()
{
	// TODO: �ڴ�����������������
	//ShellExecute(NULL, _T("open"), theApp.m_player.m_Lyrics.GetPathName().c_str(), NULL, NULL, SW_SHOWNORMAL);
	if (m_pLyricEdit != nullptr)
	{
		m_pLyricEdit->OnCancel();
		delete m_pLyricEdit;
		m_pLyricEdit = nullptr;
	}
	m_pLyricEdit = new CLyricEditDlg;
	m_pLyricEdit->Create(IDD_LYRIC_EDIT_DIALOG);
	m_pLyricEdit->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnDownloadLyric()
{
	// TODO: �ڴ�����������������
	CLyricDownloadDlg aDlg;
	aDlg.DoModal();
}


void CMusicPlayerDlg::OnLyricBatchDownload()
{
	// TODO: �ڴ�����������������
	//CLyricBatchDownloadDlg aDlg;
	//aDlg.DoModal();
	if (m_pLyricBatchDownDlg != nullptr)
	{
		m_pLyricBatchDownDlg->OnCancel();
		delete m_pLyricBatchDownDlg;
		m_pLyricBatchDownDlg = nullptr;
	}
	m_pLyricBatchDownDlg = new CLyricBatchDownloadDlg;
	m_pLyricBatchDownDlg->Create(IDD_LYRIC_BATCH_DOWN_DIALOG);
	m_pLyricBatchDownDlg->ShowWindow(SW_SHOW);

}


void CMusicPlayerDlg::OnDeleteLyric()
{
	// TODO: �ڴ�����������������
	if (!theApp.m_player.m_Lyrics.IsEmpty())
	{
		int rtn = CCommon::DeleteAFile(m_hWnd, theApp.m_player.m_Lyrics.GetPathName());		//ɾ������ļ�
		theApp.m_player.ClearLyric();		//�����ʹ���
	}
}


void CMusicPlayerDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	CPoint point1;		//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point1);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը����꣬��λ������Ļ���Ͻǵ�Ϊԭ�㣬point���Կͻ������Ͻ�Ϊԭ��
	if (m_repetemode_rect.PtInRect(point))		//����ڡ�ѭ��ģʽ���ľ��������ڵ������Ҽ����򵯳���ѭ��ģʽ�����Ӳ˵�
	{
		CMenu* pMenu = m_popup_menu.GetSubMenu(0)->GetSubMenu(15);
		if (pMenu != NULL)
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
	}
	else
	{
		m_popup_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this); //��ָ��λ����ʾ�����˵�
	}

	CDialog::OnRButtonUp(nFlags, point);
}


void CMusicPlayerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	m_repetemode_hover = (m_repetemode_rect.PtInRect(point) != FALSE);		//������ƶ�����ѭ��ģʽ�����ڵľ��ο���ʱ����m_repetemode_hover��Ϊtrue
	m_volume_hover = (m_volume_rect.PtInRect(point) != FALSE);

	CDialog::OnMouseMove(nFlags, point);
}


void CMusicPlayerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	if (m_repetemode_rect.PtInRect(point))	//����ˡ�ѭ��ģʽ��ʱ������ѭ��ģʽ
	{
		theApp.m_player.SetRepeatMode();
	}

	if (!m_show_volume_adj)		//���������ʾ����������ť�����������������ʾ����������ť
		m_show_volume_adj = (m_volume_rect.PtInRect(point) != FALSE);
	else		//����Ѿ���ʾ������������ť��������������ʱ��������������ť����ʾ
		m_show_volume_adj = (m_volume_up_rect.PtInRect(point) || m_volume_down_rect.PtInRect(point));

	if (m_show_volume_adj && m_volume_up_rect.PtInRect(point))	//�������������ť�е�������ʱ��������5
	{
		theApp.m_player.MusicControl(Command::VOLUME_UP, 5);
	}
	if (m_show_volume_adj && m_volume_down_rect.PtInRect(point))	//�������������ť�е�������ʱ������С5
	{
		theApp.m_player.MusicControl(Command::VOLUME_DOWN, 5);
	}

	CDialog::OnLButtonUp(nFlags, point);
}


HBRUSH CMusicPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//���ò���ʱ��ؼ�����ɫ
	if (pWnd == &m_time_static)
	{
		pDC->SetTextColor(m_text_color);
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


afx_msg LRESULT CMusicPlayerDlg::OnPlaylistIniComplate(WPARAM wParam, LPARAM lParam)
{
	theApp.m_player.m_loading = false;
	theApp.m_player.IniPlaylistComplate(wParam != 0);
	theApp.m_player.IniLyrics();
	ShowPlayList();
	ShowTime();
	return 0;
}