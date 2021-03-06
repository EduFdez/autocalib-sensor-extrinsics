#pragma once

#include <interfaces/CTextObserver.h>
#include <interfaces/CPlanesObserver.h>
#include <interfaces/CCorrespPlanesObserver.h>
#include <interfaces/CLinesObserver.h>
#include <interfaces/CCorrespLinesObserver.h>
#include <CPlane.h>
#include <Utils.h>

#include <QWidget>

#include <mrpt/gui/CQtGlCanvasBase.h>
#include <mrpt/img/CImage.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <vtkRenderWindow.h>
#include <opencv2/core/core.hpp>

namespace Ui {
class CViewerContainer;
}

class CViewerContainer : public QWidget, public CTextObserver, public CPlanesObserver, public CCorrespPlanesObserver, public CLinesObserver, public CCorrespLinesObserver
{
	Q_OBJECT

public:
	explicit CViewerContainer(QWidget *parent = 0);
	~CViewerContainer();

	/** Updates the sensor selection comboboxes with the available sensor labels.
	 * \param sensor_labels The list of sensor labels that are to be loaded into the comboboxes.
	 */
	void updateSensorsList(const std::vector<std::string> &sensor_labels);

	/** Update the text browswer with received text message. */
	void updateText(const std::string &);

	/** Update cloud visualizer with a point cloud.
	 * \param viewer_id the id of the visualizer.
	 * \param cloud the input cloud.
	 * \param text to be displayed in the visualizer.
	 */
	void updateCloudViewer(const int &viewer_id, const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr &cloud, const std::string &text);

	/** Updates the respective cloud visualizer(s) with a point cloud.
	 * \param sensor_id The id of the sensor the cloud belongs to.
	 * \param cloud The input cloud.
	 * \param text The text to be displayed in the visualizer.
	 */
	void updateCloudViewers(const int &sensor_id, const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr &cloud, const std::string &text);

	/**
	 * Update the result viewer with all the observations in a set, overlapped.
	 * \param cloud the input cloud.
	 * \param sensor_label the label of the sensor the cloud belongs to.
	 * \param sensor_pose the poses of the sensors in the set.
	 * \param text to be displayed in the visualizer.
	 */
	void updateSetCloudViewer(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr &cloud, const std::string &sensor_label, const Eigen::Matrix4f &sensor_pose, const std::string &text);

	/** Update the image viewer with an image.
	 * \param viewer_id the id of the image viewer to be updated.
	 * \param image pointer to the image to be displayed.
	 * \param draw indicates whether the image to be displayed is drawn with shapes.
	 */
	void updateImageViewer(const int &viewer_id, mrpt::img::CImage::Ptr &image, const bool &draw = 0);

	/** Update the respective image viewer(s) with an image.
	 * \param sensor_id The id of the sensor the image belongs to.
	 * \param image Pointer to the image to be displayed.
	 * \param draw Indicates whether the image to be displayed is drawn with shapes.
	 */
	void updateImageViewers(const int &sensor_id, mrpt::img::CImage::Ptr &image, const bool &draw = 0);


	void updateCalibConfig(const int &calib_algo_id);

	/** Check if a cloud of the specified id already exists in the viewer.
	 * \param viewer_id The id of the viewer to check for the cloud in.
	 * \param id The id of the cloud.
	 */
	bool viewerContainsCloud(const int &viewer_id, const std::string &id);

	virtual void onReceivingText(const std::string &msg);

	/** Updates the viewer with the received planes.
	 * \param viewer_id the id of the visualizer.
	 * \param planes the vector of planes to be added to the visualizer.
	 */
	virtual void onReceivingPlanes(const int &viewer_id, const std::vector<CPlaneCHull> &planes);

	/**
	 * \brief Updates the middle cloud viewer with the matched planes between each sensor pair in a set, one pair at a time.
	 * \param corresp_planes The set of corresponding planes between each sensor pair in a set that are to be visualized.
	 * \param sensor_poses The poses of the sensors in the set.
	 */
	virtual void onReceivingCorrespPlanes(std::map<int,std::map<int,std::vector<std::array<CPlaneCHull,2>>>> &corresp_planes, const std::vector<Eigen::Matrix4f> &sensor_poses);

	/**
	 * \brief Updates the image viewer with the received line segments.
	 * \param viewer_id the id of the image viewer.
	 * \param lines the vector of line segments to be drawn.
	 */
	virtual void onReceivingLines(const int &viewer_id, const std::vector<CLine> &lines);

	/**
	 * \brief Updates the middle cloud viewer with the matched 3D lines between each sensor pair in a set, one pair at time.
	 * \param corresp_lines The set of corresponding lines between each sensor pair in the set that are to be visualized.
	 * \param sensor_poses The poses of the sensors in the set.
	 */
	virtual void onReceivingCorrespLines(std::map<int,std::map<int,std::vector<std::array<CLine,2>>>> &corresp_lines, const std::vector<Eigen::Matrix4f> &sensor_poses);

private slots:

	/** Callback function to update the viewers with which sensor's observations to display. */
	void sensorIndexChanged(const int &index);

private:

	/** The sensor labels of the sensors available for display. */
	std::vector<std::string> m_sensor_labels;

	/** The ids of the sensors whose observations are to be visualized in viewers 1 and 2 repsepctively, and together in the main viewer.
	 * Only the observations of two sensors can be visualized in the viewers at a time. */
	std::vector<int> m_vsensor_ids = {0,1};

	/** An array of three PCLVisualizer objects, each linked to one viewer widget. */
	std::array<std::shared_ptr<pcl::visualization::PCLVisualizer>, 3> m_viewers;

	/** An array of image pointers that point to the images displayed in the image viewers. */
	std::array<mrpt::img::CImage::Ptr, 2> m_viewer_images;	

	/** A copy of the current set of corresponding planes to be displayed in the main viewer. */
	std::map<int,std::map<int,std::vector<std::array<int,4>>>> m_current_corresp_planes;

	Ui::CViewerContainer *m_ui;
};
