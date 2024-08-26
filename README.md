# Projet de Visualisation des Données LIDAR avec Qt et ROS2

## Introduction
Ce projet vise à développer une interface utilisateur graphique (GUI) en utilisant Qt pour visualiser en temps réel les données LIDAR obtenues à partir de ROS 2. Cette interface permet de recevoir et d'afficher les données de balayage laser (LaserScan) ainsi que les cartes d'occupation (OccupancyGrid) provenant de la simulation LIDAR sous ROS 2.

## Description
L'interface est construite en utilisant Qt5 et ROS 2 Humble. Elle offre une vue en temps réel des données LIDAR ainsi qu'une représentation cartographique. Le projet exploite la simulation disponible sur [GitHub](https://github.com/dawan0111/Simple-2D-LiDAR-Odometry) pour tester et visualiser les données LIDAR, qui sont affichées dans l'interface Qt.


## Prérequis
Avant de commencer, assurez-vous que les éléments suivants sont installés sur votre système :
- ROS 2 Humble
- Qt5 (Core, Widgets, Gui)
- CMake
- Un environnement de développement compatible (Ubuntu recommandé)


## 2. Arborescence du Projet

L'arborescence du projet est la suivante :

```bash
  ros2_ws/src/my_lidar_package/
  ├── build
  ├── CMakeLists.txt 
  ├── package.xml 
  ├── src
  │ ├── main.cpp 
```


### Fichiers Importants :
- **CMakeLists.txt** : Contient les instructions de construction pour le package ROS2 et Qt.
- **package.xml** : Déclare les dépendances du package et les informations de package.
- **main.cpp** : Implémente l'interface Qt pour la visualisation des données LIDAR et de la carte.

## 3. Dépendances

Les dépendances requises pour ce projet sont :
- **ROS2 Humble** (rclcpp, sensor_msgs, nav_msgs)
- **Qt5** (Qt5Widgets, Qt5Core, Qt5Gui)

Ces dépendances sont spécifiées dans `CMakeLists.txt` et `package.xml`.

## 4. Fonctionnalités Développées

### 4.1. Interface Qt

L'interface Qt développée comprend :
- **Visualisation LIDAR** : Une vue graphique (QGraphicsView) qui affiche les points LIDAR en temps réel.
- **Visualisation de la Carte** : Une vue de la carte affichant une grille basée sur les données d'OccupancyGrid reçues.
- **Grille** : Une grille est dessinée sur les deux vues pour une meilleure compréhension des données.

### 4.2. Abonnements ROS2

Deux abonnements ROS2 sont utilisés :
- **LaserScan** (`/scan`) : Reçoit les données LIDAR sous forme de `sensor_msgs::msg::LaserScan`.
- **OccupancyGrid** (`/map`) : Reçoit les données de la carte sous forme de `nav_msgs::msg::OccupancyGrid`.

Les données reçues sont traitées et affichées en temps réel.

## 5. Simulation Utilisée

J'ai trouvé une simulation existante sur GitHub pour une **Odométrie 2D LIDAR simple** : [Simple-2D-LiDAR-Odometry](https://github.com/dawan0111/Simple-2D-LiDAR-Odometry). Je vais exploiter cette simulation pour intégrer l'affichage des données LIDAR dans l'interface Qt. Cette simulation sera utilisée pour valider le bon fonctionnement de l'interface.

## Utilisation

### Compilation et Exécution
1. Clonez le repository et installez les dépendances :
    ```bash
    cd ~/ros2_ws/src
    git clone https://github.com/votre-utilisateur/my_lidar_package.git
    cd ..
    rosdep install --from-paths src --ignore-src -r -y
    colcon build
    ```
2. Sourcez l'environnement et lancez la simulation :
    ```bash
    source install/setup.bash
    ros2 launch my_lidar_package lidar_display.launch.py
    ```

### Lancement de l'Interface
Pour démarrer l'interface Qt et visualiser les données LIDAR :
```bash
ros2 run my_lidar_package lidar_display
```

![qt simulation](https://github.com/user-attachments/assets/c28408b8-4f89-4c08-8f27-c043bcdb91fa)

## Étape Suivante : Intégration dans l'Interface de la Navette

L'étape suivante consiste à intégrer cette interface de visualisation LIDAR dans l'interface globale de la navette autonome. Cette intégration nécessitera :

### 1. Adaptation de l'Interface
- Modifier l'interface Qt pour qu'elle s'intègre harmonieusement avec les autres composants de l'interface de la navette.
- Assurer une cohérence visuelle et fonctionnelle avec les autres éléments de l'interface utilisateur de la navette.

### 2. Fusion des Données
- Combiner les données LIDAR avec les autres données du véhicule, telles que les données de navigation, les images de caméra, etc.
- Intégrer les visualisations LIDAR dans le contexte global de l'interface, en tenant compte des autres sources d'information et des besoins de l'utilisateur.

### 3. Tests d'Intégration
- Effectuer des tests pour garantir que la visualisation des données LIDAR fonctionne correctement dans l'environnement de la navette.
- Vérifier l'interaction entre la visualisation LIDAR et les autres interfaces, et s'assurer que toutes les fonctionnalités se comportent comme prévu.
- Réaliser des tests d'intégration pour s'assurer que l'interface de visualisation LIDAR s'intègre correctement


## Conclusion

Ce projet démontre une intégration réussie entre Qt et ROS2 pour la visualisation des données LIDAR. L'interface développée permet de recevoir et d'afficher les données en temps réel, offrant ainsi une vue claire et interactive des scans LIDAR et des cartes d'occupation. Les prochaines étapes consisteront à affiner l'interface, à intégrer pleinement la simulation LIDAR, et à effectuer des tests approfondis pour garantir la robustesse de l'application.

## Références

- [ROS 2 Humble](https://docs.ros.org/en/humble/index.html)
- [Qt 5 Documentation](https://doc.qt.io/qt-5.15/)
- [Simple-2D-LiDAR-Odometry GitHub Repository](https://github.com/dawan0111/Simple-2D-LiDAR-Odometry)


