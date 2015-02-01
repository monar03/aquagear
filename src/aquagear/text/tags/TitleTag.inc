<?php
namespace aquagear\text\tags;

class TitleTag extends CoreTag {	
	public function init($value) {
	}
	
	protected function prev() {
		return '<h1>';
	}
	
	protected function fin() {
		return '</h1>';
	}
}
?>