module claro.graphics.listitem;

private import claro.base.objectsys;

struct list_item_t {}

class ListItem : CObject {
	this( list_item_t *me, CObject parent ) {
		super( parent );
		this.obj = cast(object_t*)me;
	}
}
