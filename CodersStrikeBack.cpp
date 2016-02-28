#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <limits.h>
#include <algorithm>

using namespace std;

const double delta = 1.18;

enum STATE {
  NORMAL = 0,
  RUNOFF = 1
};

struct Pod {
  int id;
  int y;
  int x;
  int vx;
  int vy;
  double speed;
  int angle;
  int degree;
  int vectorDegree;
  int nextCheckPointId;
  int nextNodeId;
  int state;

  void info() {
    fprintf(stderr,"Pod %d: y = %d, x = %d, vy = %d, vx = %d\n", id, y, x, vy, vx);
    fprintf(stderr,"Pod %d: speed = %5.2f, angle = %d\n", id, speed, angle);
  }

  void update() {
    this->speed = sqrt(this->vy*this->vy + this->vx*this->vx);
  }
};

struct CheckPoint {
  int id;
  int y;
  int x;

  CheckPoint(int id = -1, int y = -1, int x = -1) {
    this->id = id;
    this->y = y;
    this->x = x;
  }
};

struct Node {
  int id;
  int y;
  int x;
  int nextNodeId;
  int limitAcceleration;
  int limitSpeed;
  bool isCheckPoint;

  Node(int y = -1, int x = -1){
    this->y = y;
    this->x = x;
    this->limitAcceleration = 130;
    this->limitSpeed = 1000;
    this->isCheckPoint = false;
  }
};

CheckPoint checkPointList[10];
Pod myPodList[2];
Pod enemyPodList[2];
Node nodeList[100];

int laps;
int checkPointCount;
int lap;
int nodeCount;

class CodersStrikeBack {
  public:
    void init() {
      cin >> laps; cin.ignore();
      initCheckPointList();

      nodeCount = 0;
      lap = 0;
      myPodList[0].id = 0;
      myPodList[1].id = 1;
      myPodList[0].nextNodeId = 1;
      myPodList[1].nextNodeId = 1;
      enemyPodList[0].id = 2;
      enemyPodList[1].id = 3;

      //initNodeList();
      //initNodeLink();
    }

    void initCheckPointList() {
      cin >> checkPointCount; cin.ignore();
      for(int id = 0; id < checkPointCount; id++){
        int checkPointY;
        int checkPointX;
        cin >> checkPointX >> checkPointY; cin.ignore();
        checkPointList[id] = CheckPoint(id, checkPointY, checkPointX);
      }
    }

    void initNodeList() {
      int divideCount = 10;

      for(int id = 0; id < checkPointCount; id++){
        CheckPoint *from = getCheckPoint(id);
        CheckPoint *to = getCheckPoint((id+1)%checkPointCount);

        int dy = to->y - from->y;
        int dx = to->x - from->x;

        for(int i = 0; i <= divideCount; i++){
          double diff = i / (double) divideCount;
          int y = from->y + diff * dy;
          int x = from->x + diff * dx;

          Node node = createNode(y,x);

          if(i >= divideCount-1){
            node.limitAcceleration = 40 * (divideCount-i+1);
          } else if (i <= 1) {
            node.limitAcceleration = 40 * (i+1);
          }
          if(i == divideCount) {
            node.isCheckPoint = true;
          }
          nodeList[node.id] = node;
        }
      }
    }

    void initNodeLink() {
      for(int id = 0; id < nodeCount; id++){
        Node *node = getNode(id);
        node->nextNodeId = (id+1)%nodeCount;

        //fprintf(stderr,"Node: %d next %d\n", node->id, node->nextNodeId);
      }
    }

    void updateMyPod() {
      for (int id = 0; id < 2; id++) {
        Pod *pod = getMyPod(id);
        cin >> pod->x >> pod->y >> pod->vx >> pod->vy >> pod->angle >> pod->nextCheckPointId; cin.ignore();

        pod->update();

        Node *node = getNode(pod->nextNodeId);
        CheckPoint *cp = getCheckPoint(pod->nextCheckPointId);
        int targetDist = calcDist(pod->y, pod->x, cp->y, cp->x);

        pod->degree = calcDegree(pod->y, pod->x, cp->y, cp->x);
        pod->vectorDegree = calcVectorDegree(pod->vy, pod->vx);


        int range = (node->isCheckPoint)? 600 : 1200;

        //pod->info();
      }
    }

    void updateEnemyPod() {
      for (int id = 0; id < 2; id++) {
        Pod *pod = getEnemyPod(id);
        cin >> pod->x >> pod->y >> pod->vx >> pod->vy >> pod->angle >> pod->nextCheckPointId; cin.ignore();

        pod->update();
        //pod->info();
      }
    }

    void moveMyPod() {
      for(int id = 0; id < 2; id++){
        Pod *pod = getMyPod(id);
        CheckPoint *cp = getCheckPoint(pod->nextCheckPointId);

        int acceleration = 100;
        int targetDist = calcDist(pod->y, pod->x, cp->y, cp->x);

        int theta = pod->degree - pod->vectorDegree;
        int targetY = cp->y;
        int targetX = cp->x;

        int rdist, ldist;
        if(pod->degree - pod->vectorDegree >= 0){
            rdist = pod->degree - pod->vectorDegree;
        }else{
            rdist = (pod->degree+360) - pod->vectorDegree;
        }
        if(pod->vectorDegree - pod->degree >= 0){
            ldist = pod->vectorDegree - pod->degree;
        }else{
            ldist = (pod->vectorDegree+360) - pod->degree;
        }
        int tdist = min(rdist, ldist);

        if(tdist >= 45) acceleration = 50;
 
        int rotate = 4;
        if(targetDist <= 1000 || tdist >= 18){
            rotate = 18;
        }else if(targetDist <= 2000 && tdist >= 10){
            rotate = 18; 
        }else if(targetDist <= 4000 && tdist >= 10){
            rotate = 18;
        }
        int degree = calcVectorDegree(cp->y - pod->y, cp->x - pod->x);
        int relativeDegree = calcDegree(0, 0, cp->y - pod->y, cp->x - pod->x);
        //fprintf(stderr,"Pod %d: degree = %d, vectorDegree = %d, relative = %d\n", id, pod->degree, pod->vectorDegree, relativeDegree);
        //fprintf(stderr,"Pod %d: angle = %d, degree = %d, relative = %d\n", id, pod->angle, degree, relativeDegree);
        int theta1 = relativeDegree - rotate;
        int theta2 = relativeDegree + rotate;

        if(theta1 < 0) theta1 += 360;
        if(theta2 >= 360) theta2 -= 360;
        //fprintf(stderr,"y rate = %4.2f, x rate = %4.2f\n", sin(2 * theta1 * M_PI / 360.0), cos(2 * theta1 * M_PI / 360.0));

        int ly = pod->y + targetDist * sin(2 * theta1 * M_PI / 360.0);
        int lx = pod->x + targetDist * cos(2 * theta1 * M_PI / 360.0);
        int ry = pod->y + targetDist * sin(2 * theta2 * M_PI / 360.0);
        int rx = pod->x + targetDist * cos(2 * theta2 * M_PI / 360.0);

        //pod->vectorDegree = calcVectorDegree(pod->vy, pod->vx);

        //fprintf(stderr,"Pod %d: targetY = %d, targetX = %d, targetDist = %d\n", id, targetY, targetX, targetDist);

        //fprintf(stderr,"rdist = %d, ldist = %d, tdist = %d\n", rdist, ldist, tdist);

        if(tdist > 1){
            if(rdist < ldist){
                targetY = ry;
                targetX = rx;
            }else{
                targetY = ly;
                targetX = lx;
            }
        }
        if(pod->vectorDegree % 180 == 0){
            targetY = cp->y;
            targetX = cp->x;
        }

        //fprintf(stderr,"Pod %d: degree = %d, vectorDegree = %d, angle = %d, targetDist = %d\n", id, pod->degree, pod->vectorDegree, pod->angle, targetDist);

        if(abs(pod->degree - pod->vectorDegree) <= 3 && (targetDist <= 3600 && pod->speed >= 200)){
          acceleration = 0;
          cp = getCheckPoint((pod->nextCheckPointId+1)%checkPointCount);
          targetY = cp->y;
          targetX = cp->x;
        }else if(abs(pod->degree - pod->vectorDegree) <= 30){
          acceleration = 150;

          if(targetDist <= 2400 && pod->speed >= 200 && tdist <= 3){
            acceleration = 50;
            cp = getCheckPoint((pod->nextCheckPointId+1)%checkPointCount);
            targetY = cp->y;
            targetX = cp->x;
          }
          if(targetDist <= 3000 && pod->speed > 600){
            acceleration = 100;
          }
          if(targetDist >= 3500 && pod->speed <= 800){
            acceleration = 200;
          }
        }else if(abs(pod->degree - pod->angle) <= 40){
          if(targetDist >= 4000 && pod->speed <= 900){
            acceleration = 200;
          }
          if(pod->speed <= 400){
            acceleration = 200;
          }
        }else{
          acceleration = 0;

          if(targetDist >= 4000){
            acceleration = 100;
          }
        }

        printf("%d %d %d\n", targetX, targetY, acceleration);
      }
    }

    double calcDist(int y1, int x1, int y2, int x2) {
      int dy = y2 - y1;
      int dx = x2 - x1;

      return sqrt(dy*dy + dx*dx);
    }

    Node createNode(int y, int x) {
      Node node(y,x);
      node.id = nodeCount;

      nodeCount++;

      return node;
    }

    int calcDegree(int y1, int x1, int y2, int x2) {
      double radian = atan2(y2-y1, x2-x1);
      int degree = (radian * 180.0) / M_PI;
      return (degree < 0)? 360 + degree : degree;
    }

    int calcVectorDegree(int vy, int vx){
      double radian = atan2(vy, vx);
      int degree = (radian * 180.0) / M_PI;
      return (degree < 0)? 360 + degree : degree;
    }

    Pod* getMyPod(int id) {
      return &myPodList[id];
    }

    Pod* getEnemyPod(int id) {
      return &enemyPodList[id];
    }

    CheckPoint* getCheckPoint(int id) {
      return &checkPointList[id];
    }

    Node* getNode(int id) {
      return &nodeList[id];
    }
};

int main(){
  CodersStrikeBack csb;
  csb.init();

  // game loop
  while (1) {
    lap++;
    //fprintf(stderr,"%d/%d\n", lap, laps);
    csb.updateMyPod();
    csb.updateEnemyPod();
    csb.moveMyPod();
  }
}
