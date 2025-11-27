Page({

  /**
   * 页面的初始数据
   */
  data: {
    shi:'未知',
    qu:'wz',
    time:'2024-01-01',
    temp:'00',
    tianqi:'wz',
    fengxiang:'zw',
    dengji:'2',
    humi:'23',
    icon:'999',
    jiangshui:'wz',
    AQI:'wz',
    jiance:'zz',
    pa:'1211',
    PM:'333',
    see:'234'
  },
  //获取天气
  getWeather(e){
    let that=this
    wx.getLocation({
        type: 'wgs84',
        success (res) {
         console.log(res);
          const latitude = res.latitude
          const longitude = res.longitude
          const key='9ec4b623160141459b16b5d334f5140e'
          wx.request({
            url:`https://geoapi.qweather.com/v2/city/lookup?location=${longitude},${latitude}&key=${key}`,
            success(res){
                console.log(res.data.location[0].adm1);//市
                console.log(res.data.location[0].name);//qu
                that.setData({
                    shi:res.data.location[0].adm1,
                    qu:res.data.location[0].name
                })
                wx.request({
                  url: `https://devapi.qweather.com/v7/weather/now?location=${longitude},${latitude}&key=${key}`,
                  success(res){
                      console.log(res.data.now);
                      that.setData({
                        icon:res.data.now.icon,
                        tianqi:res.data.now.text,
                        temp:res.data.now.temp,
                        fengxiang:res.data.now.windDir,//fengxiang 
                        dengji:res.data.now.windScale,
                        humi:res.data.now.humidity,
                        pa:res.data.now.pressure,
                        see:res.data.now.vis,
                        jiangshui:res.data.now.precip,
                        time:res.data.updateTime.slice(11,16)                         
                      })
                      wx.request({
                        url:`https://devapi.qweather.com/v7/indices/1d?type=1,2,3,4,5&location=${longitude},${latitude}&key=${key}`,
                        success(res){
                            console.log(res);
                            that.setData({
                                AQI:res.data.daily[4].category,
                                jiance:res.data.daily[0].category,
                                PM:res.data.daily[1].category,
                              })
                        }
                      })
                  }
                })
            }
            
          })
        },

    })
  },
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    this.getWeather()
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
    
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {
    
  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {
    
  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {
    
  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {
    
  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {
    
  }
})