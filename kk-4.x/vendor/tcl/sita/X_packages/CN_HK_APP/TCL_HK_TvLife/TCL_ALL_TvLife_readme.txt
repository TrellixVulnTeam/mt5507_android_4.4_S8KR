*****************************************************************
1.APK名称：         TvLife.apk
2.APK英文名称：     TvLife
3.APK中文名称：     电视商城
	3.1APK由几个应用组成：
	 APK数目：2个。
	是否依赖系统：是（是/否）。
	
	3.2依赖的应用
	（与其他APK交互的有那些应用？有依赖的请将XX部分填写自己apk依赖的应用名称）：
	
	  3.2.1：应用1：消息盒子
		

4.APK包名:              com.tcl.shop
5.APK主入口:            com.tcl.shop.Shop       
6.厂商定义版本号：4.0
6.1: 适合的android版本：android4.0以上版本适应。
7.安装集成说明：
 7.1 预制到系统的方法：将apk放置到 /system/app ;


8 功能简介或说明:
   例如： 
   8.1 TCL生活平台项目订制。
   8.2 电视购物。

*****************************************************************
9.版本更改记录：

9.1 V4.0
         9.1.1 修改问题时间：20150407 14:00
   9.1.2 第三方应用版本名称：
             TvLife.apk
   9.1.3 在TCL的特定RT95系统出现以下bug问题：
             9.1.3.1.d （问题描述）苏宁专区中的默认配送城市是南京省南京市
             9.1.3.1.r (问题原因)  打包时地址未与苏宁数据库匹配好
             9.1.3.1.a (问题对策)  更新配送地址表
             9.1.3.1.w (修改人姓名) 李跃达
             9.1.3.1.m (修改人邮箱) liyueda@huan.tv

             9.1.3.2.d （问题描述）主界面的电视商城链接没有图标显示，并且不能进入商店
             9.1.3.2.r (问题原因)  版本有旧web版的icon入口，未完全清除
             9.1.3.2.a (问题对策)  清除旧文件包，重新集成新apk
             9.1.3.2.w (修改人姓名)  李跃达
             9.1.3.2.m (修改人邮箱)   liyueda@huan.tv

             9.1.3.3.d （问题描述）本地生活点附近筛选时一直loading
             9.1.3.3.r (问题原因)  未做数据为零时逻辑判断
             9.1.3.3.a (问题对策)  增加问题为空提示语
             9.1.3.3.w (修改人姓名)   李占红
             9.1.3.3.m (修改人邮箱)   lizhanhong@huan.tv

             9.1.3.4.d （问题描述）本地生活点显示地图，应用自动关闭
             9.1.3.4.r (问题原因)  本机未安装so库到/system/lib/下 
             9.1.3.4.a (问题对策)  把so库push到/system/lib/下 
             9.1.3.4.w (修改人姓名)   李占红
             9.1.3.4.m (修改人邮箱)   lizhanhong@huan.tv

             9.1.3.5.d （问题描述）进入电视商城-用户中心弹出提示：电视商城已停止运行
             9.1.3.5.r (问题原因)  测试用的板子估计未取到huanid 
             9.1.3.5.a (问题对策)  重启然后进入用户中心查询是否有huanid 
             9.1.3.5.w (修改人姓名)   李庆峰
             9.1.3.5.m (修改人邮箱)   liqingfeng@huan.tv

             9.1.3.6.d （问题描述）购物车中提交订单，去结算，报电视商城已停止运行
             9.1.3.6.r (问题原因)  测试用的板子估计未取到huanid 
             9.1.3.6.a (问题对策)  重启然后进入用户中心查询是否有huanid 
             9.1.3.6.w (修改人姓名)   李庆峰
             9.1.3.6.m (修改人邮箱)   liqingfeng@huan.tv


             9.1.3.7.d （问题描述）腾讯游戏大厅安装后，电商强制弹出推出提示框
             9.1.3.7.r (问题原因)  tcl 安装apk后返回的packagename为null 导致兼听安装广播空指针异常 
             9.1.3.7.a (问题对策)  加入逻辑判断代码为空时候的调用，重新提交新版本 
             9.1.3.7.w (修改人姓名)    李占红
             9.1.3.7.m (修改人邮箱)   lizhanhong@huan.tv

             9.1.3.8.d （问题描述）code值不对
             9.1.3.8.r (问题原因)  商店需求，建议改大一点 
             9.1.3.8.a (问题对策)  改code值为10 
             9.1.3.8.w (修改人姓名)    李占红
             9.1.3.8.m (修改人邮箱)   lizhanhong@huan.tv

             9.1.3.9.d （问题描述）苏宁专区、TCL附件商城，加入购物车后，点击“去结算”100%出现电商停止运行
             9.1.3.9.r (问题原因)  逻辑判断错误，代码需要优化 
             9.1.3.9.a (问题对策)  优化代码，地址为空时解决方案
             9.1.3.9.w (修改人姓名)    李占红
             9.1.3.9.m (修改人邮箱)   lizhanhong@huan.tv

             9.1.3.10.d （问题描述）当升级过程中，出现断电、断网的极端特殊情况下，无法按返回键关闭进度条
             9.1.3.10.r (问题原因)  进度条默认按返回键无法关闭
             9.1.3.10.a (问题对策)  增加容错处理。改10s后关闭进度条
             9.1.3.10.w (修改人姓名)    李占红
             9.1.3.10.m (修改人邮箱)   lizhanhong@huan.tv

             9.1.3.11.d （问题描述）自升级功能优化
             9.1.3.11.r (问题原因)  设计流程变更
             9.1.3.11.a (问题对策)  逻辑判断时改成取本地clienttype
             9.1.3.11.w (修改人姓名)    李占红
             9.1.3.11.m (修改人邮箱)   lizhanhong@huan.tv

             9.1.3.12.d （问题描述）购物车结算时，有两个不同地区配送商品，只选择其中一个无法结算
             9.1.3.12.r (问题原因)  逻辑流程需要优化
             9.1.3.12.a (问题对策)  代码已优化
             9.1.3.12.w (修改人姓名)    李占红
             9.1.3.12.m (修改人邮箱)   lizhanhong@huan.tv