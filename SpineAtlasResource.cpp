//
// Created by Raymond_Lx on 2020/6/2.
//

#include "SpineAtlasResource.h"
#include <spine/Atlas.h>

class RaiixSpineTextureLoader : public spine::TextureLoader {
private:
    SpineAtlasResource &res;
public:

    RaiixSpineTextureLoader(SpineAtlasResource &r):res(r){}

    virtual void load(spine::AtlasPage &page, const spine::String &path){
        print_line(String("Atlas tex path: ") + String(path.buffer()));
        Ref<Image> img = memnew(Image);
        ImageLoader::load_image(String(path.buffer()), img);

		Ref<ImageTexture> *p_tex = memnew(Ref<ImageTexture>(memnew(ImageTexture)));
        Ref<ImageTexture> &tex = *p_tex;
        tex->create_from_image(img);
		res.texes.append(tex);
        page.setRendererObject((void*)p_tex);
    }

    virtual void unload(void *vp_tex){
        Ref<ImageTexture> &tex = *((Ref<ImageTexture> *)vp_tex);
        tex.unref();
    }
};

Error SpineAtlasResource::load_file(const String &p_path) {
	atlas = new spine::Atlas(spine::String(p_path.utf8()), new RaiixSpineTextureLoader(*this));
    return OK;
}

SpineAtlasResource::SpineAtlasResource():atlas(NULL) {

}
SpineAtlasResource::~SpineAtlasResource() {
    if(atlas){
        delete atlas;
        atlas = NULL;
    }
}

void SpineAtlasResource::_bind_methods(){
	ClassDB::bind_method(D_METHOD("get_textures"), &SpineAtlasResource::get_textures);
	ClassDB::bind_method(D_METHOD("set_textures", "array"), &SpineAtlasResource::set_textures);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "textures"), "set_textures", "get_textures");
}

void SpineAtlasResource::set_textures(const Array &ts) {
	static bool has_start = false;
	texes = ts;
	if(has_start) return;
	has_start = true;
	reload_from_file();
	has_start = false;
}
Array SpineAtlasResource::get_textures() {
	return texes;
}