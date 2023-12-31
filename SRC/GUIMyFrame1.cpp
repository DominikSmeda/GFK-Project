#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1( wxWindow* parent)
:
MyFrame1( parent )
{
	m_button_StartStop->Disable();
}

void GUIMyFrame1::WxPanel_Repaint(wxUpdateUIEvent& event)
{
	update();
}

void GUIMyFrame1::OnText_A( wxCommandEvent& event )
{
	double arg = atoi(m_textCtrl_A->GetValue().c_str());
	curve.A = arg;
	curve.update(m_choice_coordsSystem->GetSelection());
	box.A = arg;
	box.update();
	axis.A = arg;
	axis.update();
	update();
}

void GUIMyFrame1::OnText_B( wxCommandEvent& event )
{
	double arg = atoi(m_textCtrl_B->GetValue().c_str());
	curve.B = arg;
	curve.update(m_choice_coordsSystem->GetSelection());
	box.B = arg;
	box.update();
	axis.B = arg;
	axis.update();
	update();
}

void GUIMyFrame1::OnText_C( wxCommandEvent& event )
{
	double arg = atoi(m_textCtrl_C->GetValue().c_str());
	curve.C = arg;
	curve.update(m_choice_coordsSystem->GetSelection());
	box.C = arg;
	box.update();
	axis.C = arg;
	axis.update();
	update();
}

void GUIMyFrame1::OnText_a( wxCommandEvent& event )
{
	double arg = atoi(m_textCtrl_a->GetValue().c_str());
	curve.a = arg;
	curve.update(m_choice_coordsSystem->GetSelection());
	update();
}

void GUIMyFrame1::OnText_b( wxCommandEvent& event )
{
	double arg = atoi(m_textCtrl_b->GetValue().c_str());
	curve.b = arg;
	curve.update(m_choice_coordsSystem->GetSelection());
	update();
}

void GUIMyFrame1::OnText_c( wxCommandEvent& event )
{
	double arg = atoi(m_textCtrl_c->GetValue().c_str());
	curve.c = arg;
	curve.update(m_choice_coordsSystem->GetSelection());
	update();
}

void GUIMyFrame1::OnScroll_delta( wxScrollEvent& event )
{
	curve.delta = m_slider_delta->GetValue() * 2 * M_PI / 100;
	curve.update(m_choice_coordsSystem->GetSelection());
	update();
}

void GUIMyFrame1::OnScroll_phi( wxScrollEvent& event )
{
	curve.phi = m_slider_phi->GetValue() * 2 * M_PI / 100;
	curve.update(m_choice_coordsSystem->GetSelection());
	update();
}

void GUIMyFrame1::OnScroll_rotateX( wxScrollEvent& event )
{
	update();
}

void GUIMyFrame1::OnScroll_rotateY( wxScrollEvent& event )
{
	update();
}

void GUIMyFrame1::OnScroll_rotateZ( wxScrollEvent& event )
{
	update();
}

void GUIMyFrame1::OnChoice_linePoints( wxCommandEvent& event )
{
	update();
}

void GUIMyFrame1::OnText_numberOfPoints( wxCommandEvent& event )
{
	int size = atoi(m_textCtrl_numberOfPoints->GetValue().c_str());
	if (size < 1)
	{
		size = 1;
	}
	else if (size > 5000) {
		size = 5000;
	}
	else {
		curve.setSegmentsSize(size,m_choice_coordsSystem->GetSelection());
		update();
	}

}

void GUIMyFrame1::OnCheckBox_animation( wxCommandEvent& event )
{
// TODO: Implement OnCheckBox_animation
}

void GUIMyFrame1::OnChoice_coordsSystem(wxCommandEvent& event) {
	curve.update(m_choice_coordsSystem->GetSelection());
	update();
}

void GUIMyFrame1::OnButtonClick_StartStop( wxCommandEvent& event )
{
	playAnimation = !playAnimation;
	lastTime = wxDateTime::UNow();
	if (playAnimation) {
		m_button_StartStop->SetLabel("Stop");
	}
	else {
		m_button_StartStop->SetLabel("Start");
	}
}

void GUIMyFrame1::OnText_lineLength( wxCommandEvent& event )
{
// TODO: Implement OnText_lineLength
}

void GUIMyFrame1::update() {
	wxClientDC dc1(m_panel1);
	wxBufferedDC dc(&dc1);

	dc.SetBackground(wxBrush(RGB(255, 255, 255)));
	dc.Clear();

	double alpha;
	Matrix4 mRotateX;
	alpha = m_slider_rotateX->GetValue() * 2. * M_PI / 100.;
	mRotateX.data[0][0] = 1;
	mRotateX.data[1][1] = cos(alpha);
	mRotateX.data[1][2] = -sin(alpha);
	mRotateX.data[2][1] = sin(alpha);
	mRotateX.data[2][2] = cos(alpha);

	double beta;
	Matrix4 mRotateY;
	beta = m_slider_rotateY->GetValue() * 2. * M_PI / 100.;
	mRotateY.data[0][0] = cos(beta);
	mRotateY.data[0][2] = sin(beta);
	mRotateY.data[1][1] = 1;
	mRotateY.data[2][0] = -sin(beta);
	mRotateY.data[2][2] = cos(beta);

	double gamma;
	Matrix4 mRotateZ;
	gamma = m_slider_rotateZ->GetValue() * 2. * M_PI /100.;
	mRotateZ.data[0][0] = cos(gamma);
	mRotateZ.data[0][1] = -sin(gamma);
	mRotateZ.data[1][0] = sin(gamma);
	mRotateZ.data[1][1] = cos(gamma);
	mRotateZ.data[2][2] = 1;

	Matrix4 mTranslation;
	mTranslation.data[0][0] = 1;
	mTranslation.data[0][3] = m_panel1->GetSize().GetWidth()/2;
	mTranslation.data[1][1] = 1;
	mTranslation.data[1][3] = m_panel1->GetSize().GetHeight() / 2;
	mTranslation.data[2][2] = 1;
	mTranslation.data[2][3] = 1;

	Matrix4 mScale;
	double scale = 20000;
	mScale.data[0][0] = scale / 100.0;
	mScale.data[1][1] = scale / 100.0;
	mScale.data[2][2] = scale / 100.0;


	Matrix4 mainMatrix = mTranslation * mRotateX * mRotateY * mRotateZ * mScale;


	Vector4* segments = curve.getSegments();
	Vector4* renderSegments = new Vector4[curve.getSegmentsSize()];

	for (int i = 0; i < curve.getSegmentsSize(); i++) {
		renderSegments[i] = mainMatrix * segments[i];
	}

	///////// SquareBox
	Vector4* box_lines = box.getPoints();
	Vector4* render_box_lines = new Vector4[8];

	for (int i = 0; i < 8; i++) {
		render_box_lines[i] = mainMatrix * box_lines[i];
	}
	/////////

	//////// Axis
	Vector4* axis_lines = axis.getPoints();
	Vector4* render_axis_lines = new Vector4[8];

	for (int i = 0; i < 6; i++) {
		render_axis_lines[i] = mainMatrix * axis_lines[i];
	}
	////////

	int drawMode = m_choice_linePoints->GetSelection();
	
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();

	//////// Axis drawing
	dc.SetPen(wxPen(wxColour(255, 0, 0),2));
	dc.DrawLine(render_axis_lines[0].GetX(), render_axis_lines[0].GetY(), render_axis_lines[1].GetX(), render_axis_lines[1].GetY());
	dc.SetPen(wxPen(wxColour(0, 255, 0),2));
	dc.DrawLine(render_axis_lines[2].GetX(), render_axis_lines[2].GetY(), render_axis_lines[3].GetX(), render_axis_lines[3].GetY());
	dc.SetPen(wxPen(wxColour(0, 0, 255),2));
	dc.DrawLine(render_axis_lines[4].GetX(), render_axis_lines[4].GetY(), render_axis_lines[5].GetX(), render_axis_lines[5].GetY());
	////////

	///////// Box drawing
	dc.SetPen(wxPen(wxColour(0, 0, 0), 2));
	dc.DrawLine(render_box_lines[0].GetX(), render_box_lines[0].GetY(), render_box_lines[1].GetX(), render_box_lines[1].GetY());
	dc.DrawLine(render_box_lines[1].GetX(), render_box_lines[1].GetY(), render_box_lines[2].GetX(), render_box_lines[2].GetY());
	dc.DrawLine(render_box_lines[2].GetX(), render_box_lines[2].GetY(), render_box_lines[3].GetX(), render_box_lines[3].GetY());
	dc.DrawLine(render_box_lines[3].GetX(), render_box_lines[3].GetY(), render_box_lines[0].GetX(), render_box_lines[0].GetY());
	dc.DrawLine(render_box_lines[4].GetX(), render_box_lines[4].GetY(), render_box_lines[5].GetX(), render_box_lines[5].GetY());
	dc.DrawLine(render_box_lines[5].GetX(), render_box_lines[5].GetY(), render_box_lines[6].GetX(), render_box_lines[6].GetY());
	dc.DrawLine(render_box_lines[6].GetX(), render_box_lines[6].GetY(), render_box_lines[7].GetX(), render_box_lines[7].GetY());
	dc.DrawLine(render_box_lines[7].GetX(), render_box_lines[7].GetY(), render_box_lines[4].GetX(), render_box_lines[4].GetY());
	dc.DrawLine(render_box_lines[0].GetX(), render_box_lines[0].GetY(), render_box_lines[4].GetX(), render_box_lines[4].GetY());
	dc.DrawLine(render_box_lines[1].GetX(), render_box_lines[1].GetY(), render_box_lines[5].GetX(), render_box_lines[5].GetY());
	dc.DrawLine(render_box_lines[2].GetX(), render_box_lines[2].GetY(), render_box_lines[6].GetX(), render_box_lines[6].GetY());
	dc.DrawLine(render_box_lines[3].GetX(), render_box_lines[3].GetY(), render_box_lines[7].GetX(), render_box_lines[7].GetY());
	////////


	wxPen pen(wxColour(255, 51, 153), 3);
	//pen.SetStyle(wxPENSTYLE_DOT_DASH);
	dc.SetPen(pen);

	if (drawMode == 0) {
		for (int i = 0; i < curve.getSegmentsSize() - 1; i++) {
			wxPen pen(wxColour(255, 51, 153), (renderSegments[i].GetZ() /125.0)+5);
			dc.SetPen(pen);
			dc.DrawLine(renderSegments[i].GetX(), renderSegments[i].GetY(), renderSegments[i + 1].GetX(), renderSegments[i + 1].GetY());
		}
		wxPen pen(wxColour(255, 51, 153), (renderSegments[0].GetZ() / 125.0) + 5);
		dc.SetPen(pen);
		dc.DrawLine(renderSegments[0].GetX(), renderSegments[0].GetY(), renderSegments[curve.getSegmentsSize()-1].GetX(), renderSegments[curve.getSegmentsSize()-1].GetY());
	}
	else {
		for (int i = 0; i < curve.getSegmentsSize(); i++) {
			// Rysowanie elipsy wok� punktu
			wxPoint center(renderSegments[i].GetX(), renderSegments[i].GetY());
			double depthSize = (renderSegments[i].GetZ() / 125.0) + 4;
			wxSize size(depthSize, depthSize);
			wxRect rect(center - size / 2, size);
			dc.DrawEllipse(rect);
		}
	}

	if (m_checkBox_animation->IsChecked()) {
		m_button_StartStop->Enable();
		if (playAnimation) {
			wxTimeSpan deltaTime = wxDateTime::UNow() - lastTime;
			double dt = (deltaTime.GetMilliseconds()).ToDouble() / 10.0;
			lastTime = wxDateTime::UNow();
			step += dt;
		}

		const int snakeLength = atoi(m_textCtrl_lineLength->GetValue().c_str());
		double currentSnakeLength = 0;
		
		if (step >= curve.getSegmentsSize()) {
			step = 0;
		}
		for (int i = 0; currentSnakeLength < snakeLength; i++) {
			int index1 = ((step + i) % curve.getSegmentsSize());
			int index2 = ((step + i + 1) % curve.getSegmentsSize());

			currentSnakeLength += sqrt(pow(renderSegments[index1].GetX() - renderSegments[index2].GetX(), 2) + pow(renderSegments[index1].GetY() - renderSegments[index2].GetY(), 2));

			wxPen pen(wxColour(0, 255, 53), 8);
			dc.SetPen(pen);
			dc.DrawLine(renderSegments[index1].GetX(), renderSegments[index1].GetY(), renderSegments[index2].GetX(), renderSegments[index2].GetY());

		}
	}
	else {
		m_button_StartStop->Disable();
	}


	delete[] renderSegments;
	delete[] render_axis_lines;
	delete[] render_box_lines;
}


void GUIMyFrame1::RequestAnimationFrame()
{
	update();
}

