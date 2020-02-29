圆锥与球的3D相交判断：

Y轴竖直向上

球心点为（px, py, pz）
圆锥的球心为坐标原点（0，0，0）
圆锥朝向为x轴正方向

球的已知信息：球心SphereCenter(cx, cy, cz), 半径SphereRadius
圆锥已知信息：圆锥球心(原点O)(0,0,0)，圆锥半径ConeRadius，圆锥角为ConeAngle

判断过程：
一、球心点是否在圆锥所在球体的半径内（略）
二、球心点是否在圆锥的角度内：
	1、求得圆锥顶点到球心点的向量 Delta
	2、Delta单位化为DeltaNor
	3、DeltaNor与（1，0，0）点乘（dot），根据单位向量特性可知dot的结果为Delta与X轴夹角的Cos值为CosDel
	4、求得圆锥半角StandardAngle = ConeAngle * 0.5 及其Cos值 StandardCos
	5、比较StandardCos与CosDel，根据Cos曲线特点可知，CosDel > StandardCos则球心在圆锥角度以内，反之，则在之外

三、球是否与圆锥相交
	根据推导过程，假设球与圆锥的交点为A（ax, ay, az）则球心P与A的连线PA必垂直于圆锥表面。OA与水平面的夹角为AngleBeta。根据以上假设，推导出A点坐标有如下方程：
	ax^2 = (px^2 + pz^2) * (cos(ConeAngle * 0.5)^2)
	az = ax * sin(ConeAngle * 0.5)
	ay = az * (1 / tan(AngleBeta))

	根据相交判断关系可以得出
	(点P - 点A).len < SphereRadius
	根据向量关系可以得到
	(px^2 + py^2 + pz^2) - (ax^2 + ay^2 + az^2) < SphereRadius^2
	根据上述A点坐标方程，推导出如下方程:
	ax^2 = (px^2 + pz^2) * (cos(ConeAngle * 0.5)^2)
	az^2 = (px^2 + pz^2) * (cos(ConeAngle * 0.5)^2) * sin(ConeAngle * 0.5)^2
	ay^2 = (px^2 + pz^2) * (cos(ConeAngle * 0.5)^2) * sin(ConeAngle * 0.5)^2 * (1 / tan(AngleBeta))^2
	
	已知项为px,py,pz,SphereRadius,ConeAngle,并通过P点的3D点乘（dot）求得与水平面的夹角AngleBeta
	经过公式带入及各种推导，获得如下等式
	(ax^2 + ay^2 + az^2) = (px^2 + pz^2) * (cos(ConeAngle * 0.5)^2) * (1 + sin(ConeAngle * 0.5)^2 + sin(ConeAngle * 0.5)^2 * (1 / tan(AngleBeta))^2)

	写函数即可




获取平面外一点到平面的投影点
	/*	
		平面已知点为 vPlaneVecA, vPlaneVecB, vPlaneVecC
		平面法线向量 vPlaneNor
		平面外一点 vOutPoint

		投影点	vProductPoint

		推导公式(以A点为平面原点作为计算的话，公式为)
			1、(vPlaneNor.x * (vProductPoint.x - vPlaneVecA.x)) + (vPlaneNor.y * (vProductPoint.y - vPlaneVecA.y)) + (vPlaneNor.z * (vProductPoint.z - vPlaneVecA.z)) = 0
			2、vProductPoint.y = (vPlaneNor.y / vPlaneNor.x) * (vProductPoint.x - vOutPoint.x) + vOutPoint.y
			3、vProductPoint.z = (vPlaneNor.z / vPlaneNor.x) * (vProductPoint.x - vOutPoint.x) + vOutPoint.z

			即，得到投影点的x坐标，即可确定投影点的y，z坐标.将2、3公式带入到公式1，可计算出x坐标，推导比较繁琐。公式的倒数第二步为：
			vProductPoint.x * ((vPlaneNor.x * vPlaneNor.x) + (vPlaneNor.y * vPlaneNor.y) + (vPlaneNor.z * vPlaneNor.z)) =
			(vPlaneNor.x * vPlaneNor.x * vPlaneVecA.x) + (vPlaneNor.y * vPlaneNor.y * vOutPoint.x) - (vPlaneNor.x * vPlaneNor.y * vOutPoint.y) +
			(vPlaneNor.x * vPlaneNor.y * vPlaneVecA.y) + (vPlaneNor.z * vPlaneNor.z * vOutPoint.x) - (vPlaneNor.x * vPlaneNor.z * vOutPoint.z) +
			(vPlaneNor.x * vPlaneNor.z * vPlaneVecA.z)

			求出投影点x坐标后，再将x坐标带入公式2，公式3求得投影点的y,z坐标
	*/
	//	先求出A到B，A到C的两个向量
	b3Vector3 vVecAB = vPlaneVecB - vPlaneVecA;
	b3Vector3 vVecAC = vPlaneVecC - vPlaneVecA;
	//	求得A,B,C确定的平面法线向量
	b3Vector3 vPlaneNor = vVecAB.cross(vVecAC);
	
	b3Vector3 vProductPoint = b3MakeVector3(0.0f, 0.0f, 0.0f);

	float fWithNormalSqTemp = (vPlaneNor.x * vPlaneNor.x * vPlaneVecA.x) + (vPlaneNor.y * vPlaneNor.y * vOutPoint.x) - (vPlaneNor.x * vPlaneNor.y * vOutPoint.y) + \
							(vPlaneNor.x * vPlaneNor.y * vPlaneVecA.y) + (vPlaneNor.z * vPlaneNor.z * vOutPoint.x) - (vPlaneNor.x * vPlaneNor.z * vOutPoint.z) + \
							(vPlaneNor.x * vPlaneNor.z * vPlaneVecA.z);

	vProductPoint.x = fWithNormalSqTemp / ((vPlaneNor.x * vPlaneNor.x) + (vPlaneNor.y * vPlaneNor.y) + (vPlaneNor.z * vPlaneNor.z));
	vProductPoint.y = ((vPlaneNor.y / vPlaneNor.x) * (vProductPoint.x - vOutPoint.x)) + vOutPoint.y;
	vProductPoint.z = ((vPlaneNor.z / vPlaneNor.x) * (vProductPoint.x - vOutPoint.x)) + vOutPoint.z;

获取平面外一点到平面的距离

	//	先求出A到B，A到C的两个向量
	b3Vector3 vVecAB = vPlaneVecB - vPlaneVecA;
	b3Vector3 vVecAC = vPlaneVecC - vPlaneVecA;
	//	求得A,B,C确定的平面法线向量
	b3Vector3 vPlaneNor = vVecAB.cross(vVecAC);
	//	求得P到B的向量
	b3Vector3 vVecBP = vOutSide - vPlaneVecB;
	//	计算距离
	float fDis = abs(vVecBP.dot(vPlaneNor)) / vPlaneNor.len();

