#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QSplitter>
#include <QLabel>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <cmath>

class LidarWindow : public QMainWindow {
  Q_OBJECT

public:
  LidarWindow() {
    auto *centralWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(centralWidget);

    // Create a QSplitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    // Create the lidar visualization widget
    QWidget *lidarWidget = new QWidget();
    QVBoxLayout *lidarLayout = new QVBoxLayout(lidarWidget);

    graphicsView = new QGraphicsView(this);
    graphicsScene = new QGraphicsScene(this);
    graphicsScene->setBackgroundBrush(QBrush(Qt::gray)); // Set background to gray
    graphicsView->setScene(graphicsScene);
    lidarLayout->addWidget(graphicsView);

    lidarWidget->setLayout(lidarLayout);
    splitter->addWidget(lidarWidget);

    // Create the map visualization widget
    QWidget *mapWidget = new QWidget();
    QVBoxLayout *mapLayout = new QVBoxLayout(mapWidget);

    mapLabel = new QLabel(this);
    mapLayout->addWidget(mapLabel);

    mapWidget->setLayout(mapLayout);
    splitter->addWidget(mapWidget);

    layout->addWidget(splitter);

    setCentralWidget(centralWidget);
    setWindowTitle("Lidar and Map Viewer");

    // Set scene size and adjust if necessary
    graphicsScene->setSceneRect(-3, -3, 6, 6);

    // Draw grid
    drawGrid(graphicsScene);

    // ROS 2 timer to update the UI
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &LidarWindow::updateLidarData);
    timer->start(100); // Update every 100 ms

    // ROS 2 setup
    node = std::make_shared<rclcpp::Node>("lidar_display_node");
    subscriptionLidar = node->create_subscription<sensor_msgs::msg::LaserScan>(
      "/scan", 10, std::bind(&LidarWindow::lidarCallback, this, std::placeholders::_1));
    subscriptionMap = node->create_subscription<nav_msgs::msg::OccupancyGrid>(
      "/map", 10, std::bind(&LidarWindow::mapCallback, this, std::placeholders::_1));

    // Set fixed size for the main window
    resize(1080, 720);
  }

public slots:
  void updateLidarData() {
    rclcpp::spin_some(node);
  }

private:
  void drawGrid(QGraphicsScene *scene) {
    const int gridSize = 6;
    const int step = 1;

    // Draw vertical lines
    for (int i = -gridSize; i <= gridSize; i += step) {
      QGraphicsLineItem *line = new QGraphicsLineItem(i, -gridSize, i, gridSize);
      line->setPen(QPen(Qt::black, 0.05));
      scene->addItem(line);
    }

    // Draw horizontal lines
    for (int i = -gridSize; i <= gridSize; i += step) {
      QGraphicsLineItem *line = new QGraphicsLineItem(-gridSize, i, gridSize, i);
      line->setPen(QPen(Qt::black, 0.05));
      scene->addItem(line);
    }
  }

  void lidarCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
    // Ne pas effacer toute la scène ; seulement les points lidar
    for (auto item : lidarPoints) {
      graphicsScene->removeItem(item);
      delete item;
    }
    lidarPoints.clear();

    // Debugging info
    RCLCPP_INFO(node->get_logger(), "Received LaserScan message with %lu ranges", msg->ranges.size());

    const float angle_increment = msg->angle_increment;
    float angle = msg->angle_min;

    for (float range : msg->ranges) {
        if (range > msg->range_min && range < msg->range_max) {
            float x = range * std::cos(angle);
            float y = range * std::sin(angle);
            RCLCPP_INFO(node->get_logger(), "Adding point at: (%.2f, %.2f)", x, y);
            QGraphicsEllipseItem *point = new QGraphicsEllipseItem(x, y, 0.1, 0.1);
            point->setPen(Qt::NoPen);
            point->setBrush(Qt::blue);
            graphicsScene->addItem(point);
            lidarPoints.push_back(point);
        }
        angle += angle_increment;
    }

    graphicsView->fitInView(graphicsScene->sceneRect(), Qt::KeepAspectRatio);  // Ajuster la vue pour inclure toute la scène
    graphicsView->viewport()->update();  // Forcer la mise à jour de l'affichage
  }

  void mapCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) {
    const int width = msg->info.width;
    const int height = msg->info.height;
    QImage image(width, height, QImage::Format_RGB32);

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int index = x + y * width;
        int value = msg->data[index];
        if (value == -1) {
          image.setPixel(x, height - y - 1, qRgb(128, 128, 128)); // Unknown
        } else if (value == 0) {
          image.setPixel(x, height - y - 1, qRgb(255, 255, 255)); // Free
        } else {
          image.setPixel(x, height - y - 1, qRgb(0, 0, 0)); // Occupied
        }
      }
    }

    // Draw grid on the image
    drawGridOnImage(image);

    mapLabel->setPixmap(QPixmap::fromImage(image));
  }

  void drawGridOnImage(QImage &image) {
    const int step = 10;  // Grid step size in pixels
    QPainter painter(&image);
    painter.setPen(QPen(Qt::black, 1)); // Black grid lines

    for (int x = 0; x < image.width(); x += step) {
      painter.drawLine(x, 0, x, image.height());
    }

    for (int y = 0; y < image.height(); y += step) {
      painter.drawLine(0, y, image.width(), y);
    }
  }

  QGraphicsView *graphicsView;
  QGraphicsScene *graphicsScene;
  QLabel *mapLabel;
  QTimer *timer;
  rclcpp::Node::SharedPtr node;
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscriptionLidar;
  rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr subscriptionMap;
  QList<QGraphicsEllipseItem*> lidarPoints; // Liste pour stocker les points du lidar
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  QApplication app(argc, argv);

  LidarWindow window;
  window.show();

  return app.exec();
}

#include "main.moc"

