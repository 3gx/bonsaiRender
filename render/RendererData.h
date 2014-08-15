#pragma once

#include <cassert>
#include <cmath>
#if 0
struct float2 { float x, y; };
struct float3 { float x, y, z; };
struct float4 { float x,y,z,w; };
struct int2   { int x, y; };
inline static int2 make_int2(int _x, int _y) 
{ 
  int2 v; v.x = _x; v.y = _y;; return v;
}
inline static float2 make_float2(float _x, float _y) 
{ 
  float2 v; v.x = _x; v.y = _y;; return v;
}
inline static float3 make_float3(float x, float y, float z)
{
  float3 v; v.x = x; v.y = y; v.z=z; return v;
}
inline static float4 make_float4(float x, float y, float z, float w)
{
  float4 v; v.x = x; v.y = y; v.z=z; v.w=w; return v;
}
#endif

#include <algorithm>

class RendererData
{
  public:
    typedef unsigned long long long_t;
    enum Attribute_t {
      MASS,
      VEL,
      RHO,
      H,
      NPROP};
  private:
    const int _n;
    float *_posx, *_posy, *_posz;
    long_t *_ID;
    int   *_type;
    float *_attribute[NPROP];

    float _xmin, _ymin, _zmin, _rmin;
    float _xmax, _ymax, _zmax, _rmax;

    float _globalMin;
    float _globalMax;
    
    float _attributeMin[NPROP];
    float _attributeMax[NPROP];
    
    

  public:
    RendererData(const int __n) : _n(__n)
  {
    _posx   = new float[_n];
    _posy   = new float[_n];
    _posz   = new float[_n];
    _ID     = new long_t[_n];
    _type   = new int  [_n];

    for (int p = 0; p < NPROP; p++)
      _attribute[p] = new float[_n];
  }

    ~RendererData()
    {
      delete[] _posx;
      delete[] _posy;
      delete[] _posz;
      delete[] _ID;
      delete[] _type;
      for (int p = 0; p < NPROP; p++)
        delete[] _attribute[p];
    }

    const int n() const { return _n; }

    float  posx(const int i) const { return _posx[i]; }
    float& posx(const int i)       { return _posx[i]; }
    float  posy(const int i) const { return _posy[i]; }
    float& posy(const int i)       { return _posy[i]; }
    float  posz(const int i) const { return _posz[i]; }
    float& posz(const int i)       { return _posz[i]; }

    float  attribute(const Attribute_t p, const int i) const {return _attribute[p][i]; }
    float& attribute(const Attribute_t p, const int i)       {return _attribute[p][i]; }

    int  type(const int i) const { return _type[i]; }
    int& type(const int i)       { return _type[i]; }

    long_t  ID(const long_t i) const { return _ID[i]; }
    long_t& ID(const long_t i)       { return _ID[i]; }

    void computeMinMax(const Attribute_t p)
    {
    }
    void computeMinMax()
    {
      _xmin=_ymin=_zmin=_rmin = +HUGE;
      _xmax=_ymax=_zmax=_rmax = -HUGE;
      for (int p = 0; p < NPROP; p++)
      {
        _attributeMin[p] = +HUGE;
        _attributeMax[p] = -HUGE;
      }
      for (int i = 0; i < _n; i++)
      {
        _xmin = std::min(_xmin, _posx[i]);
        _ymin = std::min(_ymin, _posy[i]);
        _zmin = std::min(_zmin, _posz[i]);
        _xmax = std::max(_xmax, _posx[i]);
        _ymax = std::max(_ymax, _posy[i]);
        _zmax = std::max(_zmax, _posz[i]);
        for (int p = 0; p < NPROP; p++)
        {
          _attributeMin[p] = std::min(_attributeMin[p], _attribute[p][i]);
          _attributeMax[p] = std::max(_attributeMax[p], _attribute[p][i]);
        }
      }
      _rmin = std::min(_rmin, _xmin);
      _rmin = std::min(_rmin, _ymin);
      _rmin = std::min(_rmin, _zmin);
      _rmax = std::max(_rmax, _xmax);
      _rmax = std::max(_rmax, _ymax);
      _rmax = std::max(_rmax, _zmax);

      for (int i = 0; i < _n; i++)
      {
        assert(_posx[i] >= _xmin && _posx[i] <= _xmax);
        assert(_posy[i] >= _ymin && _posy[i] <= _ymax);
        assert(_posz[i] >= _zmin && _posz[i] <= _zmax);
        assert(_posx[i] >= _rmin && _posx[i] <= _rmax);
        assert(_posy[i] >= _rmin && _posy[i] <= _rmax);
        assert(_posz[i] >= _rmin && _posz[i] <= _rmax);
      }
    }

    float xmin() const { return _xmin;} 
    float ymin() const { return _ymin;} 
    float zmin() const { return _zmin;} 
    float rmin() const { return _rmin;} 
    
    float xmax() const { return _xmax;} 
    float ymax() const { return _ymax;} 
    float zmax() const { return _zmax;} 
    float rmax() const { return _rmax;} 
    
    float globalMin() const {return _globalMin;}
    float globalMax() const {return _globalMax;}

    void set_globalMin(float _min) {_globalMin = _min;}
    void set_globalMax(float _max) {_globalMax = _max;}    

    float attributeMin(const Attribute_t p) const { return _attributeMin[p]; }
    float attributeMax(const Attribute_t p) const { return _attributeMax[p]; }

    void rescaleLinear(const Attribute_t p, const float newMin, const float newMax)
    {
      const float oldMin = attributeMin(p);
      const float oldMax = attributeMax(p);
     
      const float oldRange = oldMax - oldMin ;
      assert(oldRange != 0.0);

      const float slope = (newMax - newMin)/oldRange;
      float min = +HUGE, max = -HUGE;
      for (int i = 0; i < _n; i++)
      {
        attribute(p,i) = slope * (attribute(p,i) - oldMin) + newMin;  
        min = std::min(min, attribute(p,i));
        max = std::max(max, attribute(p,i));
      }
      _attributeMin[p] = min;
      _attributeMax[p] = max;
    }

    void scaleLog(const Attribute_t p, const float zeroPoint = 1.0f)
    {
      float min = +HUGE, max = -HUGE;
      for (int i = 0; i < _n; i++)
      {
        attribute(p,i) = std::log(attribute(p,i) + zeroPoint);
        min = std::min(min, attribute(p,i));
        max = std::max(max, attribute(p,i));
      }
      _attributeMin[p] = min;
      _attributeMax[p] = max;
    }
    void scaleExp(const Attribute_t p, const float zeroPoint = 1.0f)
    {
      float min = +HUGE, max = -HUGE;
      for (int i = 0; i < _n; i++)
      {
        attribute(p,i) = std::exp(attribute(p,i)) - zeroPoint;
        min = std::min(min, attribute(p,i));
        max = std::max(max, attribute(p,i));
      }
      _attributeMin[p] = min;
      _attributeMax[p] = max;
    }

    void clamp(const Attribute_t p, const float left, const float right)
    {
      assert(left  >= 0.0f && left  < 0.5f);
      assert(right >= 0.0f && right < 0.5f);

      const float oldMin = attributeMin(p);
      const float oldMax = attributeMax(p);
      const float oldRange = oldMax - oldMin ;
      assert(oldRange > 0.0f);

      const float valMin = oldMin + left *oldRange;
      const float valMax = oldMax - right*oldRange;
      assert(valMin < valMax);

      float min = +HUGE, max = -HUGE;
      for (int i = 0; i < _n; i++)
      {
        float val = attribute(p,i);
        val = std::max(valMin, std::min(valMax, val));
        attribute(p,i) = val;

        min = std::min(min, val);
        max = std::max(max, val);
      }

      _attributeMin[p] = min;
      _attributeMax[p] = max;
    }
};
