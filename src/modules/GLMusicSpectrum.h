// Copyright 2004 "Gilles Degottex"

// This file is part of "fmit"

// "fmit" is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// "fmit" is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


#ifndef _GLMusicSpectrum_h_
#define _GLMusicSpectrum_h_

#include <deque>
#include <complex>
#include <vector>
using namespace std;
// using namespace Music;
#include <QOpenGLWidget>
#include <qspinbox.h>
#include <qaction.h>
#include "View.h"
#include <memory>
#include "libs/jacobi_pd/jacobi_pd.hpp"
#include "IIRLowPass.h"

// this implements the MUSIC algorithm for frequency estimation
class GLMusicSpectrum : public QOpenGLWidget, public View
{
	Q_OBJECT

    QFont m_font;

    virtual void mouseReleaseEvent(QMouseEvent* e){View::mouseReleaseEvent(e);}
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseDoubleClickEvent(QMouseEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* e);

	vector<double> m_components;

	bool m_start_move_mouse;
	int m_press_x, m_press_y;
    double m_press_minf, m_press_maxf;

	IIRLowPass m_iirLowPass;
	int m_overlap_len; // length of the measurement blocks that are passed to the MUSIC algorithm. A fourier transform is taken of the combined block (which also is of this length) which is generated by the MUSIC algorithm.
	int m_undersample_fac;  // only every m_undersample_fac-th sample is used. This is to increase accuracy for long wavelengths (because m_overlap_len is limited). Automatically updated
	jacobi_pd::Jacobi<double, vector<double>&, vector<vector<double>>&> m_eigen_calc;
	void calcMusicSpectrum();
	void recalcFTMatrix();
	std::deque<double> m_buff_filtered;
	std::vector<double> m_buff_undersampled;
	std::vector<std::vector<double>> m_Rx_estimate;
	std::vector<double> m_evals;
	std::vector<std::vector<double>> m_evecs;
	std::vector<std::vector<double>> m_Un_Un;
	std::vector<std::complex<double>> m_temp_buff;
	std::vector<std::vector<std::complex<double>>> m_dft_matrix;

	double m_currentNoteCenterFreq;
    double m_minf, m_maxf; // [Hz]
    double m_minA, m_maxA; // [dB or amplitude depending on setting_db_scale]
    void resetaxis();
    QString m_text;

  public:
	GLMusicSpectrum(QWidget* parent);
	~GLMusicSpectrum(){}

	deque<double> buff;

	void refreshGraph();

    void setSamplingRate(int sr);

	void newNote(double noteCenterFreq);

	// settings
	QAction* setting_db_scale;
	QAction* setting_track_note;
	QSpinBox* setting_music_bins;
	QSpinBox* setting_assumed_source_count;  // this value is multiplied by 2: 1 for in-phase, 1 for 90* shifted
	QSpinBox* setting_part_length;
	QSpinBox* setting_overlap_length;

  public slots:
	void spinWinLengthChanged(int num);
    void dBScaleChanged(bool db);
	void trackNoteChanged(bool trackNote);
	void spinAssumedSourceCountChanged(int count);
	void spinPartLengthChanged(int length);
	void spinOverlapLengthChanged(int length);
	void initializeGL();
	void paintGL();
	void resizeGL( int w, int h );
};

#endif // _GLMusicSpectrum_h_

