#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1( wxWindow* parent)
:
MyFrame1( parent )
{
	
	update();
	
}

void GUIMyFrame1::OnSize(wxSizeEvent& event) {
	
	update();
	
}

void GUIMyFrame1::OnText_A( wxCommandEvent& event )
{
	double arg = atoi(m_textCtrl_A->GetValue().c_str());
	curve.A = arg;
	curve.update();
	box.A = arg;
	box.update();
	axis.A = arg;
	axis.update();
	update();
}

void GUIMyFrame1::OnText_B( wxCommandEvent& event )
{
	double arg = atoi(m_textCtrl_A->GetValue().c_str());
	curve.B = arg;
	curve.update();
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
	curve.update();
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
	curve.update();
	update();
}

void GUIMyFrame1::OnText_b( wxCommandEvent& event )
{
	double arg = atoi(m_textCtrl_b->GetValue().c_str());
	curve.b = arg;
	curve.update();
	update();
}

void GUIMyFrame1::OnText_c( wxCommandEvent& event )
{
	double arg = atoi(m_textCtrl_c->GetValue().c_str());
	curve.c = arg;
	curve.update();
	update();
}

void GUIMyFrame1::OnScroll_delta( wxScrollEvent& event )
{
	curve.delta = m_slider_delta->GetValue() * 2 * M_PI / 100;
	curve.update();
	update();
}

void GUIMyFrame1::OnScroll_phi( wxScrollEvent& event )
{
	curve.phi = m_slider_phi->GetValue() * 2 * M_PI / 100;
	curve.update();
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
		curve.setSegmentsSize(size);
		update();
	}

}

void GUIMyFrame1::OnCheckBox_animation( wxCommandEvent& event )
{
// TODO: Implement OnCheckBox_animation
}

void GUIMyFrame1::OnButtonClick_StartStop( wxCommandEvent& event )
{
// TODO: Implement OnButtonClick_StartStop
}

void GUIMyFrame1::OnText_lineLength( wxCommandEvent& event )
{
// TODO: Implement OnText_lineLength
}

void GUIMyFrame1::update() {

	wxClientDC dc(m_panel1);
	//wxBufferedDC dc(&_dc);

	double alpha;
	Matrix4 mRotateX;
	alpha = m_slider_rotateX->GetValue() * 2 * M_PI / 100;
	mRotateX.data[0][0] = 1;
	mRotateX.data[1][1] = cos(alpha);
	mRotateX.data[1][2] = sin(alpha);
	mRotateX.data[2][1] = -sin(alpha);
	mRotateX.data[2][2] = cos(alpha);

	Matrix4 mRotateY;
	alpha = m_slider_rotateY->GetValue() * 2 * M_PI / 100;
	mRotateY.data[0][0] = cos(alpha);
	mRotateY.data[0][2] = -sin(alpha);
	mRotateY.data[1][1] = 1;
	mRotateY.data[2][0] = sin(alpha);
	mRotateY.data[2][2] = cos(alpha);

	Matrix4 mRotateZ;
	alpha = m_slider_rotateZ->GetValue() * 2 * M_PI /100;
	mRotateZ.data[0][0] = cos(alpha);
	mRotateZ.data[0][1] = sin(alpha);
	mRotateZ.data[1][0] = -sin(alpha);
	mRotateZ.data[1][1] = cos(alpha);
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

	double ratio = 3;
	Matrix4 mPerspective;
	mPerspective.data[0][0] = 1;
	mPerspective.data[1][1] = 1;
	mPerspective.data[3][2] = 1.0 / ratio;

	Matrix4 mView;
	mView.data[0][0] = m_panel1->GetSize().GetWidth() / 2;
	mView.data[1][1] = m_panel1->GetSize().GetHeight() / 2;
	mView.data[0][3] = m_panel1->GetSize().GetWidth() / 2;
	mView.data[1][3] = m_panel1->GetSize().GetHeight() / 2;

	Matrix4 camMatrix = mView * mPerspective;
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


	dc.SetPen(wxPen(wxColour(255, 51, 153),2));

	if (drawMode == 0) {
		for (int i = 0; i < curve.getSegmentsSize() - 1; i++) {
			dc.DrawLine(renderSegments[i].GetX(), renderSegments[i].GetY(), renderSegments[i + 1].GetX(), renderSegments[i + 1].GetY());
		}
		dc.DrawLine(renderSegments[0].GetX(), renderSegments[0].GetY(), renderSegments[curve.getSegmentsSize()-1].GetX(), renderSegments[curve.getSegmentsSize()-1].GetY());
	}
	else {
		for (int i = 0; i < curve.getSegmentsSize(); i++) {
			dc.DrawPoint(wxPoint(renderSegments[i].GetX(), renderSegments[i].GetY()));
			// Rysowanie elipsy wok� punktu
			//wxPoint center(50, 50);
			//wxSize size(10, 10); // Rozmiar elipsy
			//wxRect rect(center - size / 2, size);
			//dc.DrawEllipse(rect);
		}
	}
	

	

	//dc.DrawRotatedText(std::to_string(renderSegments[0].GetX())+ " : " + std::to_string(renderSegments[0].GetY()), 50, 50, 1);
	//dc.DrawRotatedText(std::to_string(renderSegments[5].GetX()) + " : " + std::to_string(renderSegments[5].GetY()), 50, 100, 1);
	//dc.DrawRotatedText(std::to_string(curve.delta) + " : " + std::to_string(m_slider_delta->GetValue()), 50, 150, 1);



	delete[] renderSegments;
	delete[] render_axis_lines;
	delete[] render_box_lines;
}




