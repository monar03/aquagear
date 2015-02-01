<?php
namespace aquagear\text\tags;

class UnderLineTag extends CoreTag {
	public function init($value) {
	}
	
	protected function prev() {
		return '<u>';
	}
	
	protected function fin() {
		return '</u>';
	}
}
?>