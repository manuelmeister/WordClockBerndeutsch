class AutoBrightness {
public:
    int refresh(int light_sensor);
    float averarge_light();
    int scaled_brightness();
private:
    int light_values[30]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int light_index = 0;
    int indexed_light = 0;
};