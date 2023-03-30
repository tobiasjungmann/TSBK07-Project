
namespace scn
{
    inline std::shared_ptr<mat4> Scene::getM2W(std::size_t index) const
    {
        return modelsTransfo[index].second;
    }

    inline void Scene::setM2W(std::size_t index, std::shared_ptr<mat4> value) {
        if (index < 0)
            index += modelsTransfo.size();
        modelsTransfo[index].second = value;
    }

    inline mat4 Scene::getPreProj(std::size_t index) const {
        auto m2w = getM2W(index);
        return  camera.matrix() * (m2w == nullptr ? IdentityMatrix() : *m2w);
    }
    
    inline mat4 Scene::getPreProj(std::shared_ptr<mat4> m2w) const {
        return  camera.matrix() * (m2w == nullptr ? IdentityMatrix() : *m2w);
    }
}