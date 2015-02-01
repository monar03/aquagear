<?php
namespace aquagear\text\tags;

class BoldTag extends CoreTag {	
	public function init($value) {
	}
	
	protected function prev() {
		return '<b>';
	}
	
	protected function fin() {
		return '</b>';
	}
}
?>