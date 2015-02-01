INSERT INTO
    design(
        path,
        design_xml
    )
VALUE
    (
        :path,
        :design_xml
    )
ON DUPLICATE KEY UPDATE
    design_xml = :design_xml