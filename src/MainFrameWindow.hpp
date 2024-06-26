#ifndef MAINFRAMEWINDOW_HPP_
#define MAINFRAMEWINDOW_HPP_

#include "Config.hpp"

#include "Widgets.hpp"

namespace Base
{
	class ITraceFunction;
} // namespace BaseBase

namespace View
{
	class RobotWorldCanvas;
}

namespace Application
{
	class LogTextCtrl;

	/**
	 *
	 */
	class MainFrameWindow : public wxFrame
	{
		public:
			/**
			 *
			 * @param aTitle The title which is shown in the title bar
			 */
			explicit MainFrameWindow( const std::string& aTitle);

		protected:
			/**
			 *
			 */
			void initialise();
			/**
			 *
			 */
			wxMenuBar* initialiseMenuBar();
			/**
			 *
			 */
			wxPanel* initialiseClientPanel();
			/**
			 *
			 */
			wxSplitterWindow* initialiseSplitterWindow();
			/**
			 *
			 */
			wxPanel* initialiseLhsPanel();
			/**
			 *
			 */
			wxPanel* initialiseRhsPanel();
			/**
			 *
			 */
			wxPanel* initialiseLogPanel();
			/**
			 *
			 */
			wxPanel* initialiseConfigPanel();
			/**
			 *
			 */
			wxPanel* initialiseButtonPanel();

			wxPanel* initialiseLocalisationPanel();

		protected:

		private:
			wxPanel* clientPanel;
			wxMenuBar* menuBar;
			wxSplitterWindow* splitterWindow;

			wxPanel* lhsPanel;
			View::RobotWorldCanvas* robotWorldCanvas;

			wxPanel* rhsPanel;

			wxPanel* logPanel;
			LogTextCtrl* logTextCtrl;
			wxRadioBox* logDestination;

			wxPanel* configPanel;
			wxCheckBox* drawOpenSetCheckbox;
			wxSpinCtrl* speedSpinCtrl;
			wxRadioBox* worldNumber;

			wxPanel* buttonPanel;

			wxPanel* localisationPanel;
			wxCheckBox* kalmanFilterCheckbox;
			wxCheckBox* particleFilterCheckbox;

			void OnQuit( wxCommandEvent& anEvent);
			void OnWidgetTraceFunction( wxCommandEvent& anEvent);
			void OnStdOutTraceFunction( wxCommandEvent& anEvent);
			void OnFileTraceFunction( wxCommandEvent& anEvent);
			void OnAbout( wxCommandEvent& anEvent);

			void OnDrawOpenSet( wxCommandEvent& anEvent);
			void OnSpeedSpinCtrlUpdate( wxCommandEvent& anEvent);
			void OnWorld1( wxCommandEvent& anEvent);
			void OnWorld2( wxCommandEvent& anEvent);
			void OnWorld3( wxCommandEvent& anEvent);

			void OnStartRobot( wxCommandEvent& anEvent);
			void OnStopRobot( wxCommandEvent& anEvent);
			void OnPopulate( wxCommandEvent& anEvent);
			void OnUnpopulate( wxCommandEvent& anEvent);
			void OnStartListening( wxCommandEvent& anEvent);
			void OnSendMessage( wxCommandEvent& anEvent);
			void OnStopListening( wxCommandEvent& anEvent);

			void OnKalmanFilterCheckbox( wxCommandEvent& anEvent);
			void OnParticleFilterCheckbox( wxCommandEvent& anEvent);

			/**
			 * Draws the grid for the given panel. Useful for debugging the layout of the grid.
			 *
			 * @param aPanel with a
			 */
			void showGridFor(wxPanel* aPanel, wxGridBagSizer* aSizer);
	};
	//	class MainFrameWindow
} //namespace Application

#endif // MAINFRAMEWINDOW_HPP_
