<?php
namespace aquagear\text\tags;

class ItalicTag extends CoreTag {	
	public function init($value) {
	}
	
	protected function prev() {
		return '<i>';
	}
	
	protected function fin() {
		return '</i>';
	}
}
?>