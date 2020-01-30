package sensor.cpp;

import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.os.Build;
import android.view.WindowManager;
import android.content.Context;
import android.view.WindowManager.LayoutParams;

import java.util.List;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class GyroSensor extends Cocos2dxActivity implements SensorEventListener {
    private static SensorManager _sensorManager;
    public float[] _sensor;

    // 初期化関数
    public void Init()
    {
        // センサー取得
        _sensor = new float[3];
        _sensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        Init();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        List<Sensor> sensors = _sensorManager.getSensorList(Sensor.TYPE_GYROSCOPE);
        for (Sensor s : sensors)
        {
            _sensorManager.registerListener(this, s, SensorManager.SENSOR_DELAY_GAME);
        }
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        _sensorManager.unregisterListener(this);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy)
    {
        // TODO Auto-generated method stub
    }

    public void onSensorChanged(SensorEvent event)
    {
        _sensor[0] = event.values[0];
        _sensor[1] = event.values[1];
        _sensor[2] = event.values[2];
    }

    public float getX()
    {
        return _sensor[0];
    }
    public float getY()
    {
        return _sensor[1];
    }
    public float getZ()
    {
        return _sensor[2];
    }
}
