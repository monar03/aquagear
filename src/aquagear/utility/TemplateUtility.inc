<?php 
namespace aquagear\utility;

/**
 * テンプレート関係のユーティリティ
 * 
 * @package aquagear
 * @subpackage utility
 * 
 * @author motonari
 * @version $Id$
 */
class TemplateUtility extends \aquagear\Object {
	/**
	 * ファイルをインクルードする。
	 * 
	 * @param string $file_path 読み込むファイルパス
	 * @param array  $values 展開する変数
	 * 
	 * @return bool trueを返す。
	 */
	public static function includeTemplate($file_path, array $values=array()) {
		if(!is_file($file_path)) {
			throw new \aquagear\exception\TemplateException('cannot get template path.', '500');
		}
		
		extract($values);
		
		ob_start();
		ob_implicit_flush(0);
		
		include($file_path);
		
		$contents = ob_get_clean();
		echo $contents;
		
		return true;
	}
}
?>